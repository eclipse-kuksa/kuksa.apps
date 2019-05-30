#!/usr/bin/env python

#  Copyright (c) 2019 Eclipse KUKSA project
#
#  This program and the accompanying materials are made available under the
#  terms of the Eclipse Public License 2.0 which is available at
#  http://www.eclipse.org/legal/epl-2.0
#
#  SPDX-License-Identifier: EPL-2.0
#
#  Contributors: Robert Bosch GmbH

import json,yaml
import os
import subprocess
from datetime import datetime
import requests
from requests import Session, HTTPError

def getAppIDinHawkbit(config):
     # get the app id from hawkbit which was created by the appstore
    http = Session()
    http.auth = (config['hawkbit']['user'], config['hawkbit']['password'])
    app_response = http.get(
        url='{}/rest/v1/softwaremodules?q=name%3D%3D{}%3Bversion%3D%3D{}'.format(config['hawkbit']['url'], config['docker']['name'], config['docker']['version'])
    )
    if __handle_error(app_response) != 0:
       exit(0)  
    str_resp = app_response.content.decode("utf-8")
    respJson = json.loads(str_resp)
    contentSize = respJson['size']
    if contentSize == 1 :
       return respJson['content'][0]['id']
       print("Got the app ID in Hawkbit.")
    elif contentSize == 0 :
       print("No app with the passed name found in Hawkbit.")
       exit(0)
    else :
       print(contentSize)
       print("I am confused!. There are more than one app with the same and version. I dont know what to do. Therefore I exit!")
       exit(0)

def publish_app(config_file, upload_image=True):
    with open(config_file, mode='r') as __config_file:
       y=yaml.safe_load(__config_file)
       config = json.dumps(y)
       config = json.loads(config)
    app_image_file = None
    
    if upload_image:
        app_image_file = '{}-{}.tar'.format(config['docker']['name'], config['docker']['version'])

        app_image = config['docker']['image']
        app_image_new = '{}__{}-{}'.format(app_image, config['docker']['name'], config['docker']['version'])

        success = subprocess.run('docker tag {} {}'.format(app_image, app_image_new), shell=True).returncode
        if success != 0:
            print("Failed to re-tag the app image")
            exit(1)

        success = subprocess.run('docker save {} > {}'.format(app_image_new, app_image_file), shell=True).returncode
        if success != 0:
            print("Failed to save the docker image")
            exit(1)

        docker_container = config['docker']
        docker_container['image'] = app_image_new

    http = Session()
    http.auth = (config['hawkbit']['user'], config['hawkbit']['password'])
   
    # The app (software module) is created by the app store.
    app_id = getAppIDinHawkbit(config)

    # upload the config artifact to Hawkbit directly

    config_response = http.post(
        url='{}/rest/v1/softwaremodules/{}/artifacts'.format(config['hawkbit']['url'], app_id),
        data={
            'filename': 'docker-container.json',
        },
        files={
            'file': json.dumps(docker_container)
        },
    )
    if __handle_error(config_response) != 0:
       exit(0)  

    # upload the docker image artifact to Hawkbit directly 

    if app_image_file:
        image_response = http.post(
            url='{}/rest/v1/softwaremodules/{}/artifacts'.format(config['hawkbit']['url'], app_id),
            data={
                'filename': 'docker-image.tar',
            },
            files={
                'file': open(app_image_file, mode='rb')
            },
        )
        if __handle_error(image_response) != 0:
           exit(0)  

        os.remove(app_image_file)
    print("Uploaded the artifacts to Hawkbit.")    


def __handle_error(response):
    try:
        response.raise_for_status()
    except HTTPError as error:
        content = response.content
        if content:
            content = json.loads(content.decode("utf-8"))
            error = content.get('message')
        print(error)
        return -1
    return 0



# creates a new app category
def createNewAppCategory(config ) :
  
   headers = {
       'Content-Type': 'application/json',
       'Accept': 'application/json',
       'Authorization': config['appstore']['auth'],
   }

   data = '{\"name\" : "" }'
   data = json.loads(data)
   data['name'] = config['appstore']['category']
   response = requests.post('http://{}/api/1.0/appcategory'.format(config['appstore']['ip-address']), headers=headers, data=json.dumps(data))
   if __handle_error(response) != 0:
      print("Okay! There is already the app category in the appstore but i am not able to get its ID :(, therefore I set appCategoryID = 1. ")
      return 1  #TODO fix this, check with appstore developers.
   response_str = response.content.decode("utf-8")
   print(response_str)
   response = json.loads(response_str)
   return response['id']
   print("Created new App-category in Appstore")



# Creates new app in appstore
def createAppinAppstore(config_file) :
    with open(config_file, mode='r') as __config_file:
       y=yaml.safe_load(__config_file)
       config = json.dumps(y)
       config = json.loads(config)
       
    # create the app category in appstore.
    catID = createNewAppCategory(config)
    print(catID)
    headers = {
       'Content-Type': 'application/json',
       'Accept': 'application/json',
       'Authorization': config['appstore']['auth'],
    }
    
    data = '{\"downloadcount\": 0, "appcategory": {"id": 1} }'
    data = json.loads(data)
    data['name'] = config['docker']['name']
    data['version'] = config['docker']['version']
    data['description'] = config['docker']['description']
    data['owner'] = config['docker']['owner']
    data['publishdate'] = datetime.utcnow().isoformat()
    data['appcategory']['id'] = catID
    response = requests.post('http://{}/api/1.0/app'.format(config['appstore']['ip-address']), headers=headers, data=json.dumps(data))
    if __handle_error(response) != 0:
       print("App already exists in the appstore. Therefore no new app created.")
       return
    print("Created new app in appstore.")
  

# deletes the artifact with the passed appid and aritifact id
def deleteArtifact(config, appID, artiID) :
   http = Session()
   http.auth = (config['hawkbit']['user'], config['hawkbit']['password'])
   app_response = http.delete(
        url='{}/rest/v1/softwaremodules/{}/artifacts/{}'.format(config['hawkbit']['url'], appID, artiID),
    )
   if __handle_error(app_response) != 0:
      exit(0)


# Creates new app in appstore
def deleteExistingArtifacts(config_file) :
    with open(config_file, mode='r') as __config_file:
       y=yaml.safe_load(__config_file)
       config = json.dumps(y)
       config = json.loads(config)
    
    http = Session()
    http.auth = (config['hawkbit']['user'], config['hawkbit']['password'])

    appID = getAppIDinHawkbit(config)
    
    #get artifacts
    app_response = http.get(
        url='{}/rest/v1/softwaremodules/{}/artifacts'.format(config['hawkbit']['url'], appID),
    )
    if __handle_error(app_response) != 0:
       exit(0)
    str_resp = app_response.content.decode("utf-8")
    respJson = json.loads(str_resp)
    totalArtifacts = len(respJson)
    
    for artifact in  respJson :
       arti_id =  artifact['id']
       deleteArtifact(config, appID, arti_id )
    print("Deleted the existing artifacts in hawkbit.")



#main
if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='Publishes your docker app to kuksa Appstore')
    parser.add_argument('config_file', help="YAML config file")
    parser.add_argument('-n', '--newapp', action='store_true', default=False, help="create new app inm appstore")
    parser.add_argument('-r', '--replace', action='store_true', default=False, help="Replace artifacts in Hawkbit")
    

    args = parser.parse_args()
    newApp = args.newapp
    replace = args.replace
    if newApp :
       createAppinAppstore(args.config_file)

    if replace :
       deleteExistingArtifacts(args.config_file)
    
    publish_app(args.config_file)

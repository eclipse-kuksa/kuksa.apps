## Kuksa Apps Publisher

The kuksa publisher allows the docker apps to be published on the kuksa-appstore and also deposits the apps in the Hawkbit instance.
The kuksa publisher takes a YAML file as commandline argument, which contains details about the docker app like environment variables for the docker container, network settings etc. The YAML file also contains the IP Address and also the credentials required for the APP Store and Hawkbit instance.

Example YAML file looks something like this.

`docker :`

   `image : "IMAGE NAME & TAG"`
   
   `name : "APP-NAME"`
   
   `version : "0.1.0"`
   
   `owner : "OWNER"`
   
   `description : "DESCRIPTION ABOUT THE APP"`
   
   `config :`
   
      network_mode : "NETWORK SETTINGS
      
      environment:
      
         "DEFINE ANY ENV VARIABLE THAT THE CONTAINER REQUIRES"
 
`appstore :`

   `ip-address : "APPSTORE IP:PORT"`
   
   `category : "APP CATEGORY"`
   
   `auth : "AUTH TOKEN"`

`hawkbit :`

   `ip-address : "IP:PORT"`
   
   `target : "kuksa_Dongle"`
   
   `user : "admin"`
   
   `password : "admin"`

The `docker` section in the YAML config file contains the details related to the docker image that is built locally and stored in the local docker repository. The images could be viewed using `docker images ps`. The name tag under docker is the app-name that is used in the app-store and similarly the version. The container settings required can be put under "config". Please take a look at the .yaml files for some of the apps in this repository.

Under the section app-store and Hawkbit the location and the credentials for the the running instance could be set. TODO: Remove Username and Password from config.

### How to run

Build your docker app locally (use instructions available under docker folder of the app). Once the docker image is built, create a new APP-NAME.yaml file and update the details as mentioned above.

Execute `python3 kuksa-publisher <App.yaml> -n `  to create an alltogether new app in the appstore and correspond

Execute `python3 kuksa-publisher <App.yaml> -r `  to update the artifacts for an already existing app in the appstore. This command will update the Software Module in the Hawkbit with new Artifacts.



## Set up

1. Replace **VIRTUAL_HOST** with your domain in docker-compose.yml file to the front.environment
1. Replace server name in *docker/nginx/conf.d/resources.local.conf*
1. docker-compose up
1. Execute command `php artisan register:user` to create new user
1. Login with email


## API

Use **/api** prefix to access API  

http://resources.local/api/resources

# CENG421 File Server

In this version of UDP File server users can only see their files in their username path with their password.

for example, if user `burakcbdn` wants to reach file `a.txt` the file path `burakcbdn/a.txt` should be existing. Besides this, user should enter password to reach the files. The passwords are saved as hardcoded in to the server file.

## Adding New User

for adding new user, open `file_server.c` file, go to function `checkPass` and add username and password to the `userData` array as `username-password` value

## Adding files for user

for adding file for specific user, add user's file to the username path, for example; if you want to make `test.txt` file available for user `test123` put the file to the path `test123/test.txt`

## Usage

After creating files you want you should first run the server than the client.

### Running Server

Use any C compiler you like to run `file_server.c` file. For example with `gcc compiler` you can use `gcc file_server.c -0 file_server && ./file_server`. After running this command server will wait for requests.

### Running Client

After you run server, you can run `file_client.c` file for client. You should enter request details as arguments to the command. Again using `gcc compiler` you can run following command to run client in `local ip` to get `test.txt` file for user `burakcbdn` who is password is hopefully `123123` 

`gcc file_client.c -o file_client && ./file_client <ip> <file_name> <user_name> <password> [destination_file]`

`gcc file_client.c -o file_client && ./file_client 127.0.0.1 test.txt burakcbdn 123123`




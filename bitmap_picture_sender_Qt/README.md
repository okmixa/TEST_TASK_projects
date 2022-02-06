# Image_sender_reciver_UDP

Selecting the client-server role through the application launch parameter. 

## Server part
1. Opens a connection on the specified UDP port.
2. Waits for the client to send a JSON packet containing the "start" command.
3. Reads a picture from the bmp disk (at least 2MB in size) and sends it to the client.
4. It is necessary to provide protection against possible mixing of packets (since UDP protocol).

## Client side
1. Sends the "start" command in a JSON format packet to the server on the specified port.
2. Receives a bmp picture from the server, checks that all content is received correctly and renders it using OpenGL. 


## 1 portable verison

all required dependencies included for Windows 10 x64

[portable UDP client server](https://1drv.ms/u/s!ApW6O2P40mi2iQVci0HmphwDN5Z_?e=CcvHxW 'download binaries') 

## 2 launch udp_server

optional: you can choose another one *.bmp, otherwice default "picture.bmp" will be used.

## 3 start udp_client and press "START"
#
it will send JSON "start" to the server
#
![screen_1.png](https://github.com/rylov1986/bitmap_picture_sender_Qt/blob/main/screen_1.png)
#


## 4 wait until all raw data will been transfered

status bar show all recive and send actions.
#
![screen_2.png](https://github.com/rylov1986/bitmap_picture_sender_Qt/blob/main/screen_2.png)
##

## 5 check the result

completed picture on client side will render with OpenGL

##
![screen_3.png](https://github.com/rylov1986/bitmap_picture_sender_Qt/blob/main/screen_3.png)
#

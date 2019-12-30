#!/usr/bin/python
import sys
import serial
from socket import *


host = "localhost"
port = 21567
#buf = 1024
addr = (host,port)
UDPSock = socket(AF_INET,SOCK_DGRAM)


tty="COM4"
print("tty is ", tty)


try:
   ser = serial.Serial(tty,9600)
except:
   print("Error connecting to " , tty)
   
prevline=""
while 1:
  outline=''
  line=ser.readline()
  if line != prevline:  #some value changed
    print(line)
    outline=line
    UDPSock.sendto(outline,addr)
    prevline=line
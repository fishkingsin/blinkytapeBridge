from __future__ import division
import serial
import time
import math
import random
from BlinkyTapeV2 import BlinkyTape
import socket
import avahi
import dbus
import socket, subprocess, re

def get_ipv4_address():
	"""
	Returns IP address(es) of current machine.
	:return:
	"""
	p = subprocess.Popen(["ifconfig"], stdout=subprocess.PIPE)
	ifc_resp = p.communicate()
	patt = re.compile(r'inet\s*\w*\S*:\s*(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})')
	resp = patt.findall(ifc_resp[0])
	return resp
ips = get_ipv4_address()
print ips
UDP_IP = ips[0]
UDP_PORT = 11999

__all__ = ["ZeroconfService"]

class ZeroconfService:
		print "A simple class to publish a network service with zeroconf using avahi."

		def __init__(self, name, port, stype="_blinkytape._tcp.",
								 domain="", host="", text=""):
			self.name = name
			self.stype = stype
			self.domain = domain
			self.host = host
			
			self.port = port
			self.text = text

		def publish(self):
			bus = dbus.SystemBus()
			server = dbus.Interface(
				bus.get_object(
					avahi.DBUS_NAME,
					avahi.DBUS_PATH_SERVER),
				avahi.DBUS_INTERFACE_SERVER)
			g = dbus.Interface(
				bus.get_object(avahi.DBUS_NAME,
					server.EntryGroupNew()),
				avahi.DBUS_INTERFACE_ENTRY_GROUP)
			g.AddService(avahi.IF_UNSPEC, avahi.PROTO_UNSPEC,dbus.UInt32(0),
				self.name, self.stype, self.domain, self.host,
				dbus.UInt16(self.port), self.text)
			g.Commit()
			self.group = g
		def unpublish(self):
			self.group.Reset()

if __name__ == "__main__":

	import glob
	import optparse 

	parser = optparse.OptionParser()
	parser.add_option("-p", "--port", dest="portname",
										help="serial port (ex: /dev/ttyUSB0)", default=None)
	(options, args) = parser.parse_args()

	if options.portname != None:
		port = options.portname
	else:
		serialPorts = glob.glob("/dev/ttyACM*")
		port = serialPorts[0]

	bt = BlinkyTape(port)

	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.bind((UDP_IP, UDP_PORT))

	service = ZeroconfService(name="blinktapeBridge", port=3000)
	service.publish()
	while True:
		data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
		# print "received message:", data
		bt.serial.write(data)
    	bt.serial.flush()
    	bt.serial.flushInput()
		#do domething
	service.unpublish()
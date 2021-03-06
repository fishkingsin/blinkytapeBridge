import serial
import time

class BlinkyTape(object):
  def __init__(self, port, ledCount = 60):
    self.port = port
    self.ledCount = ledCount
    self.serial = serial.Serial(port, 115200)
    self.show() # Flush

  def sendPixel(self,r,g,b):
    data = ""
    if r == 255: r = 254
    if g == 255: g = 254
    if b == 255: b = 254
    data = chr(r) + chr(g) + chr(b)
    self.serial.write(data)
    self.serial.flush()

  def show(self):
    self.serial.write(chr(255))
    self.serial.flush()

  def displayColor(self, r, g, b):
    for i in range(0, self.ledCount):
      self.sendPixel(r,g,b)
    self.show()

  def resetToBootloader(self):
    """ Reset the blinkytape (note: this disconnects it!) """
    self.serial.setBaudrate(1200)
#    self.serial.close()
#    self.serial = serial.Serial(self.port, 1200)
#    self.serial.close()



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

  while True:

    bt.displayColor(255,0,0)
    bt.displayColor(0,255,0)
    bt.displayColor(0,0,255)
    bt.displayColor(255,255,255)
    bt.displayColor(0,0,0)


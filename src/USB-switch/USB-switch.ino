
/*
This RFduino sketch demonstrates a full bi-directional Bluetooth Low
Energy 4 connection between an iPhone application and an RFduino.
This sketch also demonstrates how to select a slower connection
interval, which will slow down the response rate, but also drastically
reduce the power consumption of the RFduino.

This sketch works with the rfduinoLedButton iPhone application.

The button on the iPhone can be used to turn the green led on or off.
The button state of button 1 is transmitted to the iPhone and shown in
the application.
*/

/*
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <RFduinoBLE.h>

int relay_1 = 5;
int relay_2 = 6;


void switchRelay(bool isOn){
  Serial.print("Switch " );
  Serial.println(isOn?"on":"off");
     digitalWrite(relay_1, isOn?HIGH:LOW);
    digitalWrite(relay_2, isOn?HIGH:LOW);
}

void setup() {
  Serial.begin(9600);
  
  Serial.println("Starting...");
  
  // led turned on/off from the iPhone app
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);

 
  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.advertisementData = "USB-Switch";
  
  // start the BLE stack
  RFduinoBLE.begin();
  Serial.println("Bluetooth on");
  switchRelay(false);
}





void loop() {
//  delay_until_button(HIGH);
//  RFduinoBLE.send(1);
//  
//  delay_until_button(LOW);
//  RFduinoBLE.send(0);

  // uncomment here to update the connection interval after the first button press  
  // RFduinoBLE_update_conn_interval(900,1000);
  
  // display the connection interval the iPhone actually selected after a button press
}


void RFduinoBLE_onConnect()
{
  // request central role use a different connection interval in the given range
  // the central role may reject the request (or even pick a value outside the range)
  // we will request something in the 900ms to 1100ms range
  // the actual rate the iPhone uses is 1098ms
  // the best way to get the connection interval you are after is trail and error
  // if the iPhone rejects the request, the connection interval will be the default (25ms)
  RFduinoBLE.updateConnInterval(900, 1100);
  Serial.println("Bluetooth onConnect()");

  // note: you cannot use delay()/RFduinoBLE.getConnInterval() here to determine which
  // connection interval the iPhone selected - getConnInterval() must be called from
  // either loop() or onReceive()
}

void RFduinoBLE_onDisconnect()
{
  Serial.println("Bluetooth onDisconnect()");

}

void RFduinoBLE_onReceive(char *data, int len)
{
  Serial.println("Bluetooth onReceive()");
  // display the connection interval the iPhone actually selected after a button press
  int connInterval = RFduinoBLE.getConnInterval();
  Serial.println(connInterval);
  
  // if the first byte is 0x01 / on / true
  if (data[0]){
    switchRelay(true);
  } else{
    switchRelay(false);
  }
    
}

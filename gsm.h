#ifndef GSM_H
#define GSM_H
#include <SoftwareSerial.h>
#define SIM800_TX_to_PIN 3 // pin on arduino
#define SIM800_RX_to_PIN 2 // pin on arduino

String atCommand, phoneNumber; // replace with your phone number after '+' 
auto gsmSerial = SoftwareSerial(SIM800_TX_to_PIN, SIM800_RX_to_PIN);  
void sendSMS(String recipient, String smsContent);

void gsmSetUp(){
     gsmSerial.begin(9600);    
     gsmSerial.println("AT");          
     delay(300);
     gsmSerial.println("AT+CMGF=1");   
     delay(300);
     gsmSerial.println("AT+CNMI=1,2,0,0,0"); 
     delay(300);
}

void gsmFunction() {
    // Check if there is data in the serial recieve buffer    
    if (Serial.available() > 0) { 
        atCommand = Serial.readString(); 
        atCommand.trim(); // remove any whitespaces acompanying the above   
    } 
    // Check if there is data sent to the GSM receive buffer 
    else if (gsmSerial.available() > 0) {    
        String sendToSerial = gsmSerial.readStringUntil('\n');
        sendToSerial.trim(); // remove any whitespaces acompanying the above 
        
        // execute the following below if GSM recieves an SMS text message from a phone
        if (sendToSerial.startsWith("+CMT:")) {              
            int firstQuote  = sendToSerial.indexOf('"');  
            Serial.print("\n\nfirstQuote position index: ");  
            Serial.print(firstQuote);  

            int comma = sendToSerial.indexOf(','); 
            Serial.print("\n\nComma position index: ");  
            Serial.print(comma);
         
            String senderNumber = sendToSerial.substring(firstQuote+1, comma-1); // extracts number in between the first quote " and comma , but excludes the last quote next to the comma e.g. // Example: +CMT: "+254779653248","","26/03/19,22:37:58+12" between "+254779653248",
            Serial.print("\n\nSender number: ");
            Serial.println(senderNumber);
            phoneNumber = senderNumber;
            delay(500);
                
            if (gsmSerial.available()) {    
                String message = gsmSerial.readStringUntil('\n');
                message.trim(); // remove any whitespaces acompanying the above 
                        
                Serial.print("\nExtracted message: ");
                Serial.println(message); // sent to serial monitor
                delay(200); 
                sendSMS(senderNumber, "phone number saved");
            }  
        }
        else{
            Serial.print("\nGSM response: ");
            Serial.println(sendToSerial); // sent to serial monitor
            delay(300); 
        }
    }   
}

// function to send SMS text messages
void sendSMS(String recipient, String smsContent){ 
      gsmSerial.println("AT+CMGF=1"); 
      gsmSerial.println("AT+CSMP=17,167,0,0");   // Set SMS parameters
      delay(300); 
    
      gsmSerial.print("AT+CMGS=\"");
      gsmSerial.print(recipient); // Specify the recipient phone number
      gsmSerial.println("\"");
      delay(300); 
    
      gsmSerial.print(smsContent); // Specify the SMS content
      delay(300);
    
      gsmSerial.write(26); // Send the SMS by writing the ASCII code 26  
      delay(500);
}
#endif

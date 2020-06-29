import processing.serial.*;
import java.util.Date;

Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port
PrintWriter output;


void setup()
{
  // I know that the first port in the serial list on my mac
  // is Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  myPort = new Serial(this, Serial.list()[2], 9600);
  
  output = createWriter("C:\\Users\\DEMJ\\OneDrive - Hanzehogeschool Groningen\\Bureaublad\\ESKA project\\demonstrator\\data.txt");
  output.println("Time\tHumidity Out\tTemperature Out\tHumidity Int\tTemperature In\tHumidity Boxt\tTemperature Box\tWind Speed\tTemperature Towel\tHumidity Towel");

}

void draw()
{
  if ( myPort.available() > 0) 
  {  // If data is available,
  Date d = new Date();
  long current=d.getTime()/1000;
  output.print(current);
  output.print("\t");
  val = myPort.readString();         // read it and store it in val
  println(val); //print it out in the console
  output.println(val); //print it out in the console

  } 
}

void keyPressed() {
    output.flush();  // Writes the remaining data to the file
    output.close();  // Finishes the file
    exit();  // Stops the program
}

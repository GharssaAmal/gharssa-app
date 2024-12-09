#include "arduino.h"

Arduino::Arduino()
{
    data="";//Contient les données reçues de l'Arduino, initialisées à une chaîne vide.
    arduino_port_name="";//Nom du port série auquel l'Arduino est connecté, initialisé à une chaîne vide.
    arduino_is_available=false;//Indique si l'Arduino est disponible ou non,
    serial=new QSerialPort;// signifie que vous créez une nouvelle instance de la classe QSerialPort, qui permet de communiquer avec des ports série.utuliser pour configurer et contrôler une connexion série
}

QString Arduino::getarduino_port_name()//retourne le nom du port série  sur lequel l'Arduino est connecté. l5it lazra9
{
    return arduino_port_name;
}

QSerialPort *Arduino::getserial()//Cette méthode retourne un pointeur vers l'objet QSerialPort.Cela permet d'accéder directement à l'objet serial pour des configurations
{
   return serial;
}
int Arduino::connect_arduino()
{

    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()){
           if(serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()){
               if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id && serial_port_info.productIdentifier()
                       == arduino_uno_producy_id) {
                   arduino_is_available = true;//La carte Arduino est trouvée
                   arduino_port_name=serial_port_info.portName();
               } } }
        qDebug() << "arduino_port_name is :" << arduino_port_name;
        if(arduino_is_available){ // configuration de la communication ( débit...)
            serial->setPortName(arduino_port_name);
            if(serial->open(QSerialPort::ReadWrite)){
                serial->setBaudRate(QSerialPort::Baud9600); // débit : 9600 bits/s
                serial->setDataBits(QSerialPort::Data8); //Longueur des données : 8 bits,
                serial->setParity(QSerialPort::NoParity); //1 bit de parité optionnel
                serial->setStopBits(QSerialPort::OneStop); //Nombre de bits de stop : 1
                serial->setFlowControl(QSerialPort::NoFlowControl);
                return 0;
            }
            return 1;
        }
        return -1;
}

int Arduino::close_arduino()

{

    if(serial->isOpen()){
            serial->close();
            return 0;
        }
    return 1;


}


 QByteArray Arduino::read_from_arduino()
{
    if(serial->isReadable()){
         data=serial->readAll(); //récupérer les données reçues

         return data;
    }
 }


void Arduino::write_to_arduino( QByteArray d)

{

    if(serial->isWritable()){
        serial->write(d);  // envoyer des donnés vers Arduino
    }else{
        qDebug() << "Couldn't write to serial!";
    }


}

//Author is Erik de Zeeuw
#include "hwlib.hpp"
#include "clock.hpp"
#include "sinus.hpp"

int main( void ){	  
   namespace target = hwlib::target;
   
   auto scl = target::pin_oc( target::pins::scl );
   auto sda = target::pin_oc( target::pins::sda );
   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
   
   auto display = hwlib::glcd_oled( i2c_bus, 0x3c );  

   auto minutesButton = target::pin_in( target::pins::d6 );
   auto hoursButton = target::pin_in( target::pins::d5 );
     
   clock clock1 = clock( display, hoursButton, minutesButton);
   
   clock1.start();


}
/*****************************************************************************/
//  LowLevelExample.ino
//  Hardware:      Grove - 6-Axis Accelerometer&Gyroscope
//	Arduino IDE:   Arduino-1.65
//	Author:	       Lambor
//	Date: 	       Oct,2015
//	Version:       v1.0
//
//  Modified by:
//  Data:
//  Description:
//
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/

#include "LSM6DS3.h"
#include "Wire.h"
#include "SPI.h"

uint16_t errorsAndWarnings = 0;

//Create instance of LSM6DS3Core
LSM6DS3Core myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A


long lastTime;
long lastInterval;

float accelX,            accelY,             accelZ,            // units m/s/s i.e. accelZ if often 9.8 (gravity)
      gyroX,             gyroY,              gyroZ,             // units dps (degrees per second)
      gyroDriftX,        gyroDriftY,         gyroDriftZ,        // units dps
      gyroRoll,          gyroPitch,          gyroYaw,           // units degrees (expect major drift)
      gyroCorrectedRoll, gyroCorrectedPitch, gyroCorrectedYaw,  // units degrees (expect minor drift)
      accRoll,           accPitch,           accYaw,            // units degrees (roll and pitch noisy, yaw not possible)
      complementaryRoll, complementaryPitch, complementaryYaw;  // units degrees (excellent roll, pitch, yaw minor drift)

void setup() {
    //Init Serial port
    Serial.begin(9600);
    while (!Serial);

    //Call .beginCore() to configure the IMU
    if (myIMU.beginCore() != 0) {
        Serial.print("\nDevice Error.\n");
    } else {
        Serial.print("\nDevice OK.\n");
    }

    uint8_t dataToWrite = 0;  //Temporary variable

    //Setup the accelerometer******************************
    dataToWrite = 0; //Start Fresh!
    dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_100Hz;
    dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_8g;
    dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_104Hz;

    //Now, write the patched together data
    errorsAndWarnings += myIMU.writeRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, dataToWrite);

    //Set the ODR bit
    errorsAndWarnings += myIMU.readRegister(&dataToWrite, LSM6DS3_ACC_GYRO_CTRL4_C);
    dataToWrite &= ~((uint8_t)LSM6DS3_ACC_GYRO_BW_SCAL_ODR_ENABLED);

}

void loop() {
    int16_t accelX,accelY,accelZ;
    //Get all parameters
    Serial.print("\nAccelerometer Counts:\n");
    
    long currentTime = micros();
    lastInterval = currentTime - lastTime; // expecting this to be ~104Hz +- 4%
    lastTime = currentTime;

    //Acelerometer axis X
    if (myIMU.readRegisterInt16(&accelX, LSM6DS3_ACC_GYRO_OUTX_L_XL) != 0) {
        errorsAndWarnings++;
    }
    Serial.print(" X = ");
    Serial.println(accelX);

    //Acelerometer axis Y
    if (myIMU.readRegisterInt16(&accelY, LSM6DS3_ACC_GYRO_OUTY_L_XL) != 0) {
        errorsAndWarnings++;
    }
    Serial.print(" Y = ");
    Serial.println(accelY);

    //Acelerometer axis Z
    if (myIMU.readRegisterInt16(&accelZ, LSM6DS3_ACC_GYRO_OUTZ_L_XL) != 0) {
        errorsAndWarnings++;
    }
    Serial.print(" Z = ");
    Serial.println(accelZ);

    accRoll = atan2(accelY, accelZ) * 180 / M_PI;
    accPitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180 / M_PI;
  
    float lastFrequency = (float) 1000000.0 / lastInterval;
    gyroRoll = gyroRoll + (gyroX / lastFrequency);
    gyroPitch = gyroPitch + (gyroY / lastFrequency);
    gyroYaw = gyroYaw + (gyroZ / lastFrequency);
  
    gyroCorrectedRoll = gyroCorrectedRoll + ((gyroX - gyroDriftX) / lastFrequency);
    gyroCorrectedPitch = gyroCorrectedPitch + ((gyroY - gyroDriftY) / lastFrequency);
    gyroCorrectedYaw = gyroCorrectedYaw + ((gyroZ - gyroDriftZ) / lastFrequency);
  
    complementaryRoll = complementaryRoll + ((gyroX - gyroDriftX) / lastFrequency);
    complementaryPitch = complementaryPitch + ((gyroY - gyroDriftY) / lastFrequency);
    complementaryYaw = complementaryYaw + ((gyroZ - gyroDriftZ) / lastFrequency);
  
    complementaryRoll = 0.98 * complementaryRoll + 0.02 * accRoll;
    complementaryPitch = 0.98 * complementaryPitch + 0.02 * accPitch;

    Serial.print(gyroRoll);
    Serial.print(',');
    Serial.print(gyroPitch);
    Serial.print(',');
    Serial.print(gyroYaw);
    Serial.print(',');
    
    Serial.print(gyroCorrectedRoll);
    Serial.print(',');
    Serial.print(gyroCorrectedPitch);
    Serial.print(',');
    Serial.print(gyroCorrectedYaw);
    Serial.print(',');
    Serial.print(accRoll);
    Serial.print(',');
    Serial.print(accPitch);
    Serial.print(',');
    Serial.print(accYaw);
    Serial.print(',');
    Serial.print(complementaryRoll);
    Serial.print(',');
    Serial.print(complementaryPitch);
    Serial.print(',');
    Serial.print(complementaryYaw);
    Serial.println("");

    Serial.println();
    Serial.print("Total reported Errors and Warnings: ");
    Serial.println(errorsAndWarnings);

    delay(1000);
}

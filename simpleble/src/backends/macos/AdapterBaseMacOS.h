#pragma once

#import <CoreBluetooth/CoreBluetooth.h>
#import <Foundation/Foundation.h>
#import <IOBluetooth/IOBluetooth.h>

#include <functional>
#import "AdapterMac.h"

@interface AdapterBaseMacOS : NSObject<CBCentralManagerDelegate>

@property NSString* uuid;

- (bool)isBluetoothEnabled;

- (instancetype)init:(SimpleBLE::AdapterMac*)adapter;

- (void*)underlying;

- (void)powerOn;
- (void)powerOff;
- (bool)isPowered;

- (void)scanStart;
- (void)scanStop;
- (bool)scanIsActive;

- (NSString*)address;

@end

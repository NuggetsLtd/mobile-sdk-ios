#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "RNPermissions.h"
#import "RNPermissionHandlerFaceID.h"
#import "RNPermissionHandlerPhotoLibraryAddOnly.h"
#import "RNPermissionHandlerMicrophone.h"
#import "RNPermissionHandlerSpeechRecognition.h"
#import "RNPermissionHandlerPhotoLibrary.h"
#import "RNPermissionHandlerCamera.h"
#import "RNPermissionHandlerMediaLibrary.h"
#import "RNPermissionHandlerNotifications.h"

FOUNDATION_EXPORT double RNPermissionsVersionNumber;
FOUNDATION_EXPORT const unsigned char RNPermissionsVersionString[];


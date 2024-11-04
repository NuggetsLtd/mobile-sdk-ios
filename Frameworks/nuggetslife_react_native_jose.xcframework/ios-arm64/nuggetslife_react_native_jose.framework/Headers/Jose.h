
#ifdef RCT_NEW_ARCH_ENABLED
#import "RNJoseSpec.h"

@interface Jose : NSObject <NativeJoseSpec>
#else
#import <React/RCTBridgeModule.h>

@interface Jose : NSObject <RCTBridgeModule>
#endif

@end

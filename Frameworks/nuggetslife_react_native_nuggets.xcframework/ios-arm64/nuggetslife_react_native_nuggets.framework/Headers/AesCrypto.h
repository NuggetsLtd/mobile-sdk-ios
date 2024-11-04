#import <Foundation/Foundation.h>

@interface AesCrypto : NSObject
+ (NSString *) pbkdf2:(NSString *)password salt: (NSString *)salt iterations: (NSInteger)iterations keyLength: (NSInteger)keyLength;
+ (NSString *) randomBytes: (NSInteger)size;
@end

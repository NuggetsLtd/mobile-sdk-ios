<p align="center">
  <a href="https://nuggets.life">
    <img src="./assets/nuggets-logo.svg" alt="Nuggets Ltd" width="250">
  </a>
</p>

# Nuggets Mobile SDK (iOS)
Mobile (iOS) SDK for interaction with the Nuggets Platform.

This SDK provides an Identity Wallet for Self-Sovereign Identity (SSI). 

## Integration

### Pre-requisites
1. Install [CocoaPods](https://cocoapods.org/)
2. Initialise CocoaPods: `pod init`
  
### Install `NuggetsSDK` dependency
1. Add SDK dependency to your _Podfile_:
    ```pod
    pod 'NuggetsSDK', :git => 'https://github.com/NuggetsLtd/mobile-sdk-ios.git', :tag => 'v1.1.0'
    ```
2. Download & install packages:
   ```pod
    pod install --repo-update
   ```

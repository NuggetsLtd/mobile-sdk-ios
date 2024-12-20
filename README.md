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

### Create New App

1. Open XCode
2. Goto: `File` -> `New` -> `Project...`
3. Select `App` & set the following options:
   1. Product Name: `NuggetsDemo`
   2. Interface: `Storyboard`
   3. Language: `Swift`
4. In a terminal window, from the root App directory:

```bash
pod init
```

5. Add `NuggetsSDK` to Podfile:

```podspec
pod 'NuggetsSDK', :git => 'https://github.com/NuggetsLtd/mobile-sdk-ios.git', :branch => 'main'
```

6. The install the Pods:

```bash
pod install --repo-update
```

7. Close the project in XCode
8. Open `NuggetsDemo.xcworkspace` in XCode (the workspace contains Pods project)
9. Fix `Undefind symbols:` error:
   1. Update XCode `NuggetsDemo` project `Build Phases` for `Link Binary With Libraries`:
      1. Click "+"
      2. Search for "MobileCore"
      3. Add `MobileCoreServices.framework`
      4. Ensure it's the top item in the list
10. Fix `rsync.samba` error:
    1. Update XCode `NuggetsDemo` project `Build Settings` for `ENABLE_USER_SCRIPT_SANDBOXING` to `No`
11. Update the `NuggetsDemo` -> `NuggetsDemo` -> `ViewController.swift` file to extend `ViewController` from `NuggetsSDKViewController`:

```swift
//
//  ViewController.swift
//  NuggetsDemo
//
//  Created by Nuggets Ltd on 01/02/2024.
//

import NuggetsSDK

class ViewController: NuggetsSDKViewController {
```

13. Update the `NuggetsDemo` -> `NuggetsDemo` -> `AppDelegate.swift` file to add `window` variable:

```swift
//
//  AppDelegate.swift
//  NuggetsDemo
//
//  Created by Nuggets Ltd on 01/02/2024.
//

import UIKit

@main
class AppDelegate: UIResponder, UIApplicationDelegate {
    var window: UIWindow?
```

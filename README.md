<p align="center">
  <a href="https://nuggets.life">
    <img src="./assets/nuggets-logo.svg" alt="Nuggets Ltd" width="250">
  </a>
</p>

# Nuggets Mobile SDK – iOS Full Integration Guide

This document lists ALL changes needed to integrate the Nuggets Mobile SDK into a fresh iOS host app.

---
## 0. Overview & Architecture (Why These Steps Exist)
- Delivery: The SDK ships as a collection of XCFrameworks (some React Native powered internals) surfaced via a CocoaPod.
- Presentation: You host and present `NuggetsSDKViewController` modally (full‑screen) from your app.
- Core Capability REQUIREMENTS: Camera access, NFC capability (with entitlement + usage string), and microphone/biometric support are REQUIRED for the Nuggets identity/liveness/document flows. These are not optional if you intend to run the full SDK journey.
- Isolation: Keeping the SDK logic in its own controller avoids polluting the host app navigation stack.
- React Native Internals: Supporting frameworks (`React`, `yoga`, etc.) must be linkable (hence static linking choice and not embedding duplicates).
- NFC & Sensitive Capabilities: iOS enforces entitlement + Info.plist usage description + runtime permission (where applicable). Missing any piece causes silent feature failure or rejection during App Store review.
- Security / Privacy: Remove only truly unused ancillary permissions. Do NOT remove required Camera / NFC / Microphone / FaceID descriptions.

---
## 1. Prerequisites
- Xcode 14+
- [CocoaPods](https://cocoapods.org/) installed (`brew install cocoapods`)
- iOS 13.0+ target (raise if corporate baseline higher)
- Swift project (Objective‑C works via bridging header)
- Git access allowed (SDK pulled from GitHub)

Why: Minimum iOS 13 aligns with modern Scene lifecycle + required APIs in dependencies.

---
## 2. Create a New Project
1. Xcode → File → New → Project → App
2. Product Name: `NuggetsDemo` (example)
3. Interface: Storyboard OR SwiftUI (guide uses UIKit for simplicity)
4. Language: Swift
5. Finish

---
## 3. Initialise CocoaPods (Dependency Manager)
```bash
pod init
```
Why: Pod manages transitive React Native support libraries + simplifies future updates.

---
## 4. Add NuggetsSDK to Podfile (Static Linking Rationale)
Edit your `Podfile`:
```ruby
use_frameworks! :linkage => :static

pod 'NuggetsSDK', :git => 'https://github.com/NuggetsLtd/mobile-sdk-ios.git'
```
Then install (fetch remote spec + dependencies):
```bash
pod install --repo-update
```
Why static: Avoid duplicate symbol / embedding issues with React Native style frameworks and keep single copy in final app. Do not switch to dynamic unless you fully validate runtime.

---
## 5. Open the Workspace (NOT the Project)
Close the `.xcodeproj` and open the `.xcworkspace` in Xcode:
```bash
open NuggetsDemo.xcworkspace
```
Why: Workspace contains the Pods project; opening the `.xcodeproj` will break build references.

---
## 6. AppDelegate Setup
Add this property to your `AppDelegate.swift`:
```swift
var window: UIWindow?
```
Why: Some internal flows may expect a window reference for presentation contexts (esp. during early lifecycle or permission prompts).

---
## 7. Example ViewController (Minimal Launch Flow)
A minimal example using UIKit and the SDK:
```swift
import UIKit
import NuggetsSDK

class ViewController: UIViewController, NuggetsSDKViewControllerDelegate {
    override func viewDidLoad() {
        super.viewDidLoad()
        let launchButton = UIButton(type: .system)
        launchButton.setTitle("Launch Nuggets", for: .normal)
        launchButton.addTarget(self, action: #selector(launchNuggetsSDK), for: .touchUpInside)
        launchButton.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(launchButton)
        NSLayoutConstraint.activate([
            launchButton.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            launchButton.centerYAnchor.constraint(equalTo: view.centerYAnchor)
        ])
    }
    @objc func launchNuggetsSDK() {
        // If a future SDK version exposes configuration (e.g. environment), supply it here before presentation.
        let controller = NuggetsSDKViewController()
        controller.modalPresentationStyle = .overFullScreen
        controller.delegate = self
        present(controller, animated: true)
    }
    // MARK: - NuggetsSDKViewControllerDelegate
    func nuggetsSDKDidFinish() { print("Nuggets SDK finished successfully.") }
    func nuggetsSDKDidFail(with error: Error) {
        print("Nuggets SDK failed: \(error.localizedDescription)")
        let alert = UIAlertController(title: "Error", message: error.localizedDescription, preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "OK", style: .default))
        dismiss(animated: true) { self.present(alert, animated: true) }
    }
}
```
### 7.1 SwiftUI Host Variant (Optional)
```swift
import SwiftUI
import NuggetsSDK

struct ContentView: View {
    @State private var presenting = false
    var body: some View {
        Button("Launch Nuggets") { presenting = true }
        .fullScreenCover(isPresented: $presenting) {
            NuggetsWrapper()
        }
    }
}

struct NuggetsWrapper: UIViewControllerRepresentable {
    func makeUIViewController(context: Context) -> NuggetsSDKViewController { NuggetsSDKViewController() }
    func updateUIViewController(_ uiViewController: NuggetsSDKViewController, context: Context) {}
}
```
Why: Keeps SwiftUI integration clean while reusing underlying UIKit controller.

---
## 8. Required Frameworks & Libraries (Purpose)
In Xcode, go to your target → General → Frameworks, Libraries, and Embedded Content. Add (do NOT embed):

| Item | Purpose | Required |
|------|---------|----------|
| CoreNFC.framework | Access to NFC passport / chip reading flows | Yes |
| NFCPassportReader.xcframework | High-level MRZ/NFC handling (passport chip) | Yes (full identity) |
| NuggetsSDK.xcframework | Core Nuggets feature set | Yes |
| Pods_NuggetsSDK.xcframework | Aggregated CocoaPods build artefact (static linkage) | Yes |
| React / RCTFabric / RCTSettings / yoga / CoreModules | React Native runtime primitives used internally | Yes |
| folly / CoreModules.xcframework | Support libs (C++ helpers, module system) | Yes |
| MobileCoreServices.framework | Legacy type identifiers (resolves undefined symbol issues) | Yes |

All listed frameworks are required for the complete Nuggets experience including liveness + NFC. Do not remove CoreNFC even if initially testing—flows will fail when NFC steps are invoked.

---
## 9. Info.plist Configuration (Rationale & Trim Guidance)
Add the following keys/values to your `Info.plist`:

Group keys into: Required, Conditional (feature‑driven), Network Exceptions.

| Key | Status | Reason |
|-----|--------|--------|
| NSCameraUsageDescription | Required | Document & facial capture |
| NSMicrophoneUsageDescription | Required | Liveness / video capture requires audio session |
| NSFaceIDUsageDescription | Required (if device has Face ID) | Biometric auth convenience & security |
| NFCReaderUsageDescription | Required | Passport / NFC identity step |
| NSMotionUsageDescription | Strongly Recommended | Anti-spoof / liveness accuracy |
| NSPhotoLibrary(Add)UsageDescription | Optional | Persisting captured images locally |
| NSSpeechRecognitionUsageDescription | Optional | Only if speech step utilized |
| NSLocation*UsageDescription | Optional | Geo signal (only if business need) |
| NSBluetooth*UsageDescription | Optional | Peripheral interaction if used |
| NSCalendarsUsageDescription | Optional | Remove unless explicit requirement |
| NSAppleMusicUsageDescription | Optional / Likely remove | Not needed for core flows |
| ITSAppUsesNonExemptEncryption | Required | Export compliance declaration |

If you omit a Required key, related flows will fail or be blocked by system policy.

---
## 10. NFC Entitlements (Why)
Create an entitlements file (e.g. `NuggetsDemo.entitlements`) and add:

Entitlement enables system NFC session APIs. Without it: controller creation may succeed but NFC reading prompts never appear. Add only if using NFC.

Entitlements file (`NuggetsDemo.entitlements`):
```xml
<key>com.apple.developer.nfc.readersession.formats</key>
<array>
    <string>TAG</string>
</array>
```
Add file under target → Signing & Capabilities.

---
## 11. Build Settings & Fixes (Root Causes Explained)
- In `Build Phases` → `Link Binary With Libraries`, add `MobileCoreServices.framework` (move to top if you see undefined symbols).
- In `Build Settings`, set `ENABLE_USER_SCRIPT_SANDBOXING` to `No` if you see `rsync.samba` errors.
- If using OpenSSL, add a post-build script to strip bitcode:
```sh
FRAMEWORK_PATH="${TARGET_BUILD_DIR}/${FRAMEWORKS_FOLDER_PATH}/OpenSSL.framework/OpenSSL"
if [[ -f "$FRAMEWORK_PATH" ]]; then
  echo "Stripping bitcode from OpenSSL"
  xcrun bitcode_strip -r "$FRAMEWORK_PATH" -o "$FRAMEWORK_PATH"
fi
```

| Issue | Symptom | Fix | Reason |
|-------|---------|-----|--------|
| Undefined symbols (MobileCoreServices) | Linker errors | Add `MobileCoreServices.framework` | RN / dependent code references legacy UTType constants |
| `rsync.samba` script failure | Build phase error | Set `ENABLE_USER_SCRIPT_SANDBOXING = No` | Some pod scripts require broader fs access (older tooling) |
| Bitcode strip warnings (OpenSSL) | Archive warnings | Add post-build strip script | Ensures no stale bitcode segments in bundled 3rd-party build |
| Duplicate symbols | Link failures | Ensure single static linkage; do not embed frameworks twice | Avoids multiple object copies of RN core |

Post‑build (only if OpenSSL present):
```sh
FRAMEWORK_PATH="${TARGET_BUILD_DIR}/${FRAMEWORKS_FOLDER_PATH}/OpenSSL.framework/OpenSSL"
if [[ -f "$FRAMEWORK_PATH" ]]; then
  echo "Stripping bitcode from OpenSSL"
  xcrun bitcode_strip -r "$FRAMEWORK_PATH" -o "$FRAMEWORK_PATH"
fi
```

---
## 12. Runtime Permissions Strategy
Request as late as possible (just before feature use), but ensure REQUIRED items ultimately are granted for a successful end‑to‑end identity flow:
- Camera / Microphone: REQUIRED – ask before document or liveness capture.
- NFC: REQUIRED – entitlement + usage string must exist; system sheet appears at session start.
- Face ID / Biometrics: REQUIRED for seamless secure re-entry; system handles prompt.
- Motion: Recommended; if denied, SDK may fall back to reduced anti‑spoof checks.
- Others (Location, Bluetooth, Speech, Photo Library): Only if those optional features are enabled in your product context.

If user permanently denies a REQUIRED permission, guide them to Settings with a clear rationale message.

---
## 13. Validation Checklist
| Step | Verify |
|------|--------|
| Pod install | No unresolved dependencies |
| Launch button | SDK UI displays |
| Core permissions | Camera/Microphone/NFC/FaceID available and granted path tested |
| Info.plist | All REQUIRED usage descriptions present |
| Entitlements | NFC entitlement present |
| ATS | Exceptions scoped only to required domains |
| Release build | No missing symbols / warnings escalated |
| Device test | Physical device liveness + NFC pass |
| App Store readiness | Privacy strings clear, user-centric |

---
## 14. Updating the SDK (Safe Process)
1. Review CHANGELOG & release notes (breaking flags?).
2. Adjust Podfile version/tag if version pinning used (if no tag specified you track latest on that branch / commit).
3. `pod repo update && pod install`.
4. Clean build (`Shift+Cmd+K`) then build & run on device.
5. Regression: launch, complete identity flow, NFC (if used), error path.
6. Re‑archive (ensure no new warnings promoted to errors).

Rollback: Revert Podfile change + `pod install` (CocoaPods keeps previous cache).

---
## 15. Troubleshooting
| Problem | Likely Cause | Action |
|---------|--------------|--------|
| Blank / black SDK screen | Missing required framework or presentation not full‑screen | Confirm list in Section 8; ensure `modalPresentationStyle = .overFullScreen` |
| NFC sheet never appears | Missing entitlement or device lacks NFC | Add entitlements file; test on NFC-capable hardware |
| App Store rejection (excess permissions) | Unused Info.plist keys retained | Remove unused privacy keys & resubmit |
| Linker duplicate symbols | Framework embedded + statically linked | Remove embedding; keep static linkage only |
| Network calls blocked | Overly strict ATS after hardening | Reintroduce scoped ATS domain exception |
| Pod install slow / stale | Local specs outdated | `pod repo update` then reinstall |

---
## 16. Production Hardening
- Prune unused Info.plist usage descriptions and ATS exceptions.
- Validate privacy copy: must explain direct user benefit.
- Run on slow network & airplane toggle tests (resilience).
- Verify memory footprint (Instruments) during multi-step identity flow.
- Monitor logs for deprecated API warnings.

---
## 17. Support
Include: SDK version (commit/tag), device model, iOS version, reproduction steps, concise console log excerpt, whether NFC / biometrics enabled.

---
## 18. Quick Reference (Minimal Required Set)
Absolute minimum for full Nuggets identity + liveness + NFC journey:
- NSCameraUsageDescription
- NSMicrophoneUsageDescription
- NFCReaderUsageDescription
- NSFaceIDUsageDescription (if device supports Face ID)
- ITSAppUsesNonExemptEncryption
- NFC entitlement file (`com.apple.developer.nfc.readersession.formats`)
Remove only optional keys once confirmed unused. Omitting any required key breaks core flow.

---
> Required: Camera, Microphone, NFC (usage + entitlement), FaceID (if available). Do not remove these; the Nuggets SDK depends on them for end‑to‑end identity verification.


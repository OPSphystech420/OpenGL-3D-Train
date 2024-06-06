
#ifdef __APPLE__

#include <Carbon/Carbon.h>

void scrollCallback(CGEventRef event, void* userInfo) {
    CGEventType type = CGEventGetType(event);
    if (type == kCGEventScrollWheel) {
        double deltaY = CGEventGetDoubleValueField(event, kCGScrollWheelEventDeltaAxis1);
        if (deltaY > 0) {
            camera_distance -= 0.5f;
        } else {
            camera_distance += 0.5f;
        }
        if (camera_distance < 1.0f) {
            camera_distance = 1.0f;
        } else if (camera_distance > 100.0f) {
            camera_distance = 100.0f;
        }
    }
}

void checkPermissions() {
    if (!AXIsProcessTrusted()) {
        fprintf(stderr, "no permission");
        exit(1);
    }
}

#endif

/*
 
 // Add -framework Cocoa to run.sh
 
 // Add to main() for scroller events
 
#ifdef __APPLE__
    checkPermissions();
    CGEventMask eventMask = CGEventMaskBit(kCGEventScrollWheel);
    CFMachPortRef eventTap = CGEventTapCreate(kCGSessionEventTap,
                                              kCGHeadInsertEventTap,
                                              kCGEventTapOptionDefault,
                                              eventMask,
                                              (CGEventTapCallBack)scrollCallback,
                                              NULL);
    if (!eventTap) {
        fprintf(stderr, "event tap failed");
        exit(1);
    } else {
        CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
        CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
        CGEventTapEnable(eventTap, true);
    }
#endif


*/

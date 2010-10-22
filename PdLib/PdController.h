/*
 PdController.h
 PdLib v0.3

 Copyright 2010 Bryan Summersett. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are
 permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of
 conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list
 of conditions and the following disclaimer in the documentation and/or other materials
 provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY BRYAN SUMMERSETT ``AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BRYAN SUMMERSETT OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 The views and conclusions contained in the software and documentation are those of the
 authors and should not be interpreted as representing official policies, either expressed
 or implied, of Bryan Summersett.

 */

#import <Foundation/Foundation.h>
#import "AudioOutput.h"

@protocol PdControllerDelegate
- (void)bounceStopped:(NSString *)file;
@end


// This is a somewhat malformed singleton.
// First set externs, openfiles, and libdir to the desired values BEFORE
// calling "start" on PdController. Any changes to these properties 
// after starting will not be reflected until PD is either stopped or restarted.

@interface PdController : NSObject
{
@private
    NSThread        *pdThread;
    NSArray         *externs;
    NSArray         *openfiles;
    NSString        *libdir;
    NSInteger       soundRate; // in Hz
    NSInteger       blockSize; // essentially, how many samples to run per DSP tick. Defaults to 256.
                               // Must be a multiple of 64 (Pure Data's default)
    NSInteger       nOutChannels; // 1 or 2: If we want mono or stereo out.
    NSInteger       nInChannels; // 1 or 2: mono / stereo mic
    AudioCallbackFn callbackFn; // Function to call back for every DSP tick. Called BEFORE DSP rendering

    AudioFileID     audioFileID;
    ExtAudioFileRef audiofile;
    NSString        *file;

    id<PdControllerDelegate> delegate;
}

+ (PdController *)sharedController;
- (int)openFile:(NSString *)path;
- (void)start;
- (void)restart;
- (void)stop;
- (OSStatus)startBounce:(NSString *)inRecordFile;
- (void)stopBounce;
- (void)endBounce;
- (void)sendMsg:(const char *)msg len:(size_t)len;
- (int)recvReady;
- (void)recvMsg:(char **)data len:(size_t *)len;

@property (nonatomic, copy) NSArray *externs;
@property (nonatomic, copy) NSArray *openfiles;
@property (nonatomic, copy) NSString *libdir;
@property (nonatomic, assign) NSInteger soundRate;
@property (nonatomic, assign) NSInteger blockSize;
@property (nonatomic, assign) NSInteger nOutChannels;
@property (nonatomic, assign) NSInteger nInChannels;
@property (nonatomic, assign) AudioCallbackFn callbackFn;
@property (nonatomic, assign) AudioFileID audioFileID;
@property (nonatomic, assign) ExtAudioFileRef audiofile;
@property (nonatomic, assign) id<PdControllerDelegate> delegate;
@end

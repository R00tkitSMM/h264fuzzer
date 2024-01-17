./fuzzer  -in  in -out out -t 1000  -target_env DYLD_INSERT_LIBRARIES=./flip.dylib  -delivery_dir /Volumes/RAMDisk -file_extension mov \
-instrument_module AV1DecoderSW \
-instrument_module AppleGVAHEVCEncoder \
-instrument_module AppleMCTF	\
-instrument_module AppleProResHWEncoder	\
-instrument_module JPEGVideoDecoder \
-instrument_module AppleAVEEncoder \
-instrument_module AppleGVAVPXDecoder \
-instrument_module AppleMP4V \
-instrument_module AppleProResRAWCodec \
-instrument_module JPEGVideoEncoder \
-instrument_module AppleAfterburnerProResDecoder \
-instrument_module AppleH263 \
-instrument_module AppleProResCodec \
-instrument_module AppleVideoDecoder \
-instrument_module VCPHEVC \
-instrument_module AppleGVAHEVCDecoder \
-instrument_module AppleH264SW \
-instrument_module AppleProResHWDecoder	\
-instrument_module AppleVideoEncoder \
-target_module vtdecode -target_method _fuzz -nargs 1 -iterations 5000 -persist -loop -cmp_coverage -- ./vtdecode @@

# -instrument_module VideoToolbox 
# -target_env DYLD_INSERT_LIBRARIES=/usr/lib/libgmalloc.dylib
#  -target_env DYLD_INSERT_LIBRARIES=./flip.dylib 
# -mute_child

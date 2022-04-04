#import <AppKit/AppKit.h>
#import "mfilemanager.h"

declare_reflectable_class(MOSXBundle)
class MOSXBundle : public MExtends<MOSXBundle, MBundle> {
public:
    static void install();

public:
    MVector<uint8_t>::ptr loadAsset(const std::string &path) override;

    std::string documentDirectory () override;
    std::string temporaryDirectory() override;
    
private:
    NSBundle *mBundle = nil;
};

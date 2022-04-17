#import <Foundation/Foundation.h>
#import "mfilemanager.h"

declare_reflectable_class(MMACBundle)
class MMACBundle : public MExtends<MMACBundle, MBundle> {
public:
    static void install();

public:
    MVector<uint8_t>::ptr loadAsset(const std::string &path) override;

    std::string documentDirectory () override;
    std::string temporaryDirectory() override;
    
private:
    NSBundle *mBundle = nil;
};

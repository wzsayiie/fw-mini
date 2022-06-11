#import <Foundation/Foundation.h>
#import "mfilemanager.h"

minikit_class(MMACBundle, MBundle) {
public:
    static void install();

public:
    MVector<uint8_t>::ptr onLoadAsset(const std::string &path) override;

    std::string onGetBundleDirectory   () override;
    std::string onGetDocumentDirectory () override;
    std::string onGetTemporaryDirectory() override;
    
private:
    NSBundle *mAssetBundle = nil;
};

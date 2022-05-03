#import <Foundation/Foundation.h>
#import "mfilemanager.h"

m_class(MMACBundle, MBundle) {
public:
    static void install();

public:
    MVector<uint8_t>::ptr loadAsset(const std::string &path) override;

    std::string documentDirectory () override;
    std::string temporaryDirectory() override;
    
private:
    NSBundle *mBundle = nil;
};

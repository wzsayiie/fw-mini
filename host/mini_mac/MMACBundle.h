#import <Foundation/Foundation.h>
#import "mfilemanager.h"

m_class(MMACBundle, MBundle) {
public:
    static void install();

public:
    MVector<uint8_t>::ptr onLoadResource(const std::string &path) override;

    std::string onGetResBundleDirectory() override;
    std::string onGetDocumentDirectory () override;
    std::string onGetTemporaryDirectory() override;
    
private:
    NSBundle *mResBundle = nil;
};

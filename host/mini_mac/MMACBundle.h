#import <Foundation/Foundation.h>
#import "mbundle.h"

m_class(MMACBundle, MBundle) {
public:
    static void install();

public:
    MData::ptr onLoadResource(const std::string &path) override;

    std::string onGetResBundleDirectory() override;
    std::string onGetDocumentDirectory () override;
    std::string onGetTemporaryDirectory() override;
    
private:
    NSBundle *mResBundle = nil;
};

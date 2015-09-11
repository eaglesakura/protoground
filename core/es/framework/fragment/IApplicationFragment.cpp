#include    "IApplicationFragment.h"
#include    "ApplicationFragmentController.hpp"

namespace es {
/**
 * アタッチされているアプリを取得する
 */
IApplication * IApplicationFragment::getApplication() const {
    assert(controller);
    return controller->getApplication();
}

}

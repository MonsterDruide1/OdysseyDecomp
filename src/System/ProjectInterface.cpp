#include "Library/System/SystemKit.h"
#include "System/Application.h"

al::SystemKit* alProjectInterface::getSystemKit() {
    return Application::instance()->getSystemKit();
}

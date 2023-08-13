#include "Library/System/SystemKit.h"
#include "game/System/Application.h"

al::SystemKit* alProjectInterface::getSystemKit() {
    return Application::instance()->getSystemKit();
}

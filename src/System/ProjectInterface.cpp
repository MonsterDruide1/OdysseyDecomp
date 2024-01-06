#include "System/ProjectInterface.h"

#include "System/Application.h"

al::SystemKit* alProjectInterface::getSystemKit() {
    return Application::instance()->getSystemKit();
}

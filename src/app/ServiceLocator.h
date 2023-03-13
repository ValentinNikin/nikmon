#pragma once

#include <memory>

#include <Hypodermic/ContainerBuilder.h>

class ServiceLocator {
private:
    ServiceLocator() = default;
    static std::unique_ptr<ServiceLocator> _instance;
public:
    static ServiceLocator* getInstance();

    template<typename T>
    std::shared_ptr<T> resolve() {
        return _container->resolve<T>();
    }

    template<typename T>
    std::shared_ptr<T> resolve(const T&) {
        return _container->resolve<T>();
    }

private:
    void init();
    std::shared_ptr<Hypodermic::Container> _container;
};
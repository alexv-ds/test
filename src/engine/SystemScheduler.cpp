#include "SystemScheduler.hpp"
#include <filesystem>
#include <format>
#include <regex>
#include <sstream>
#include <yaml-cpp/yaml.h>
#include "log.hpp"

#include <ranges>

namespace {
  class LambdaSystem final : public engine::System {
  public:
    static std::shared_ptr<System> create(std::function<void(entt::registry& reg)> func) {
      return std::make_shared<LambdaSystem>(std::move(func));
    }

    explicit LambdaSystem(std::function<void(entt::registry&)>&& func) :
        func_(std::move(func)) {}

    void update() override { this->func_(registry()); }

  private:
    std::function<void(entt::registry&)> func_;
  };

} // namespace

namespace engine {


  SystemScheduler::SystemScheduler(const std::filesystem::path& config_path,
                                   std::shared_ptr<entt::registry> ecs_registry) :
      ecs_registry(std::move(ecs_registry)) {
    this->load_orders(config_path);
  }

  SystemScheduler::~SystemScheduler() {
    for (auto& system : this->systems) {
      try {
        system.second->stop();
      }
      catch (const std::exception& e) {
        LOG_ERROR("System stop error: {}", e.what());
      }
    }
  }

  void SystemScheduler::add_system(std::string_view name,
                                   std::shared_ptr<System> system) {
    std::string system_name{name.begin(), name.end()};
    if (const auto it = this->system_map.find(system_name);
        it != this->system_map.end()) {
      throw std::runtime_error(std::format("System '{}' already exists", name));
    }
    system->init(this->ecs_registry);
    system->start(); // TODO: start according to the order in which the systems are called
    this->system_map.emplace(std::move(system_name), std::move(system));
    this->need_reindex = true;
    LOG_TRACE("System '{}' added", name);
  }

  void SystemScheduler::add_system(std::string_view name,
                                   std::function<void(entt::registry&)> system) {
    add_system(name, LambdaSystem::create(system));
  }

  void SystemScheduler::run_systems() {
    if (this->need_reindex) {
      this->reindex_systems();
      this->need_reindex = false;
    }

    if (this->systems.empty()) {
      LOG_CRITICAL("Has no systems for run");
      std::exit(EXIT_FAILURE);
    }

    for (const auto& [name, system] : this->systems) {
      try {
        system->update();
      }
      catch (const std::exception& e) {
        LOG_ERROR("System update error. System - {}, Error - {}", name, e.what());
      }
    }
  }

  void SystemScheduler::load_orders(const std::filesystem::path& config_path) {
    if (!exists(config_path)) {
      throw std::runtime_error(
        std::format("File '{}' does not exist", config_path.string()));
    }

    if (!is_regular_file(config_path)) {
      throw std::runtime_error(
        std::format("File '{}' is not regular file", config_path.string()));
    }

    YAML::Node doc = YAML::LoadFile(config_path.string());

    auto groups = doc["groups"];
    auto order = doc["order"];

    if (!order.IsSequence()) {
      throw std::runtime_error(R"(property "order" is not array)");
    }

    const std::regex group_regex{R"(^group\.(.+)$)"};
    std::smatch group_match;
    for (auto it = order.begin(); it != order.end(); ++it) {
      if (!it->IsScalar()) {
        throw std::runtime_error(R"(invalid "order" format)");
      }
      if (auto value = it->as<std::string>();
          std::regex_match(value, group_match, group_regex)) {
        if (!groups.IsMap()) {
          throw std::runtime_error(R"(property "groups" is not map)");
        }
        YAML::Node group = groups[group_match[1].str()];
        if (group.IsNull()) {
          continue;
        }
        if (!group.IsSequence()) {
          throw std::runtime_error(
            std::format(R"(group "{}" is not sequence)", group_match[1].str()));
        }
        for (auto group_it = group.begin(); group_it != group.end(); ++group_it) {
          this->order.push_back(group_it->as<std::string>());
        }
      }
      else {
        this->order.push_back(std::move(value));
      }
    }
  }

  void SystemScheduler::reindex_systems() {
    this->systems.clear();
    this->systems.reserve(this->system_map.size());
    std::set<std::string> not_runned_systems;
    for (const auto& name : this->system_map | std::views::keys) {
      not_runned_systems.insert(name);
    }

    for (const std::string& system_name : this->order) {
      if (const auto it = this->system_map.find(system_name);
          it != this->system_map.end()) {
        this->systems.emplace_back(system_name, it->second);
        not_runned_systems.erase(system_name);
      }
    }

    if (this->systems.empty()) {
      LOG_CRITICAL("Has no systems for run");
      std::exit(EXIT_FAILURE);
    }


    // order report
    {
      std::ostringstream report;
      report << "Systems reindexed:" << std::endl;
      for (std::size_t i = 0; i < this->systems.size(); ++i) {
        report << "    " << i + 1 << ") " << this->systems[i].first;
        if (i != this->systems.size() - 1) {
          report << std::endl;
        }
      }
      LOG_DEBUG(report.str());
    }

    // waring report
    for (const std::string& system_name : not_runned_systems) {
      LOG_WARN("System registered, but not present in execution order config - {}",
               system_name);
    }
  }


} // namespace engine

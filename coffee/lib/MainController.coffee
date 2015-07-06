remote = require("remote")
ConfigManager = remote.require("./lib/ConfigManager")

module.exports =
class MainController
    @$inject = ["$scope"]

    constructor: (@scope) ->
        configManager = ConfigManager.getInstance()
        @scope.configs = configManager.configs
        @scope.selectedConfig = configManager.selectedConfig

        @scope.configSelected = @configSelected

    configSelected: () =>
        configManager = ConfigManager.getInstance()
        cfg = @scope.selectedConfig
        configManager.selectConfig(cfg.id)
        configManager.save()
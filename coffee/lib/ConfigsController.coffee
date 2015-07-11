ConfigManager = require("./ConfigManager")

module.exports =
class ConfigsController
    @instance = null
    @$inject = ["$scope", "$state"]

    constructor: (@scope, @state) ->
        ConfigsController.instance = @
        @configManager = ConfigManager.getInstance()
        # We don't want to edit the actual configs until we save them
        @scope.configs = JSON.parse(JSON.stringify(@configManager.configs))
        @scope.save = @save
        @scope.addConfig = @addConfig
        @scope.removeConfig = @removeConfig

    save: () =>
        @configManager.configs = JSON.parse(JSON.stringify(@scope.configs))
        @configManager.save()
        @state.go("main")

    addConfig: () =>
        cfg =
            id: Math.round(Math.random() * 10000)
            name: "New configuration"
            exePath: "C:\\path\\to\\exe"
        @scope.configs.push(cfg)
        @scope.currentConfig = cfg

    removeConfig: () =>
        if not @scope.currentConfig?
            return

        cfg = d for d in @scope.configs when d.id == @scope.currentConfig.id
        if cfg?
            @scope.currentConfig = null
            @scope.configs.splice(@scope.configs.indexOf(cfg), 1)
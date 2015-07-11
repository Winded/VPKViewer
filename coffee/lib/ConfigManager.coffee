module.exports =
class ConfigManager
    @instance: null
    @getInstance: () ->
        @instance = new ConfigManager() if not @instance?
        return @instance

    constructor: () ->
        @configs = []
        @selectedConfig = null

    selectConfig: (id) ->
        cfg = d for d in @configs when d.id == id
        if cfg?
            @selectedConfig = cfg
        else
            throw new Error("Config with id #{id} not found")

    load: () ->
        data = window.localStorage.config
        if not data?
            return false
        data = JSON.parse(data)
        @configs = data.configs
        @selectedConfig = cfg for cfg in data.configs when cfg.id == data.selectedConfig
        return true

    save: () ->
        data =
            configs: @configs
            selectedConfig: @selectedConfig.id
        json = JSON.stringify(data)
        window.localStorage.config = json
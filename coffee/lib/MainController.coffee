module.exports =
class MainController
    @$inject = ["$scope"]

    constructor: (@scope) ->
        @scope.text1 = "Column 1"
        @scope.text2 = "Column 2"
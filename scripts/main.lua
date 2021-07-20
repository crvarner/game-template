
player = {
    Idle = {},
    Position = {
        x = 100,
        y = 200,
        w = 100,
        h = 100,
    },
    Sprite = {
        spritesheet = "assets/player.png",
    }
}

function init()
    createEntity(player)
end
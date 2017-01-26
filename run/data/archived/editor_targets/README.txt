What's this and how to use it?

The content of this folder (the *.json files) will be processed during
initialisation of targets. With this the editor system can be extended with new
items: guns, buffs, walls, ...
The files describe a _single_ target. A descriptor looks something like this:
{
    "target_name": "target_type_name",
    "name":"unique_name"
    "setters":
    {
        ...
    }
}
The descriptions consist of 2 parts:a generic and a specialised part.
Generic part:
"target_name": string that defines type of target, see later
"name": string that defines a unique name of the target

The specialised part is target specific.
Currently the following targets and specialised parts are supported.

WallTarget: walls and wall like objects like water, ...
"target_name":"wall_target"

"setters":
{
    "cursor_id": string
    "actor_id": string
}

CtfSoldierSpawnTarget: soldier descriptor for CTF game mode
"target_name":"ctf_soldier_spawn_target"

"setters":
{
    "cursor_id": "ctf_player", <- valid example value
    "team": "blue" <- "blue" or "red"
}

SoldierSpawnTarget: soldier descriptor for non CTF game modes
"target_name":"soldier_spawn_target"

"setters":
{
    "cursor_id": "player" <- valid example value
}

PickupTarget: descriptor for weapons, pickups and buffs
"target_name":"revive_pickup"

"setters":
{
        "type_id":  <- possible values: "weapon" for weapons, "normal" for pickups like grenade, "buff" for buffs
        "content_id":
}

FlagSpawnTarget: descriptor for CTF game mode's flag platform
"target_name":"flag_spawn_target"

"setters":
{
    "cursor_id": "platform", <- valid example value
    "team": <- possible values: "blue", "red"
}

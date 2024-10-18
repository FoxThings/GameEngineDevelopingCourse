local ecs = require "ecs"

local function time_tick(it)
    for t, ent in ecs.each(it) do
        t.time = t.time + it.delta_time
    end
end

local function delayed_destroy(it)
    for pos, t, dest, ent in ecs.each(it) do
        if t.time > dest.toDestroy then
            pos.y = 10000.0
        end
    end
end

local function collision(it)
    for posA, obsA, entA in ecs.each(it) do
        for posB, obsB, entB in ecs.each(it) do
            if entA ~= entB then
                if (posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) + (posA.z - posB.z) * (posA.z - posB.z) < 4.0 then
                    obsA.isCollided = true
                    obsB.isCollided = true

                    if obsA.isBullet then
                        obsB.isCollidedByBullet = true
                    end

                    if obsB.isBullet then
                        obsA.isCollidedByBullet = true
                    end
                else
                    obsA.isCollided = false
                    obsB.isCollided = false
                end
            end
        end
    end
end

local function bounce(it)
    for pos, vel, obs, ent in ecs.each(it) do
        if obs.isCollided then
            vel.x = vel.x * -1.0
            pos.x = pos.x + vel.x
            obs.isCollided = false
        end
    end
end

local function shooted(it)
    for pos, obs, ent in ecs.each(it) do
        if obs.isCollidedByBullet then
            pos.y = pos.y + 4.0
        end
    end
end

ecs.system(time_tick, "TimeTick", ecs.OnUpdate, "Timer")
ecs.system(delayed_destroy, "DelayedDestroy", ecs.OnUpdate, "Position, Timer, ToDestroy")
ecs.system(collision, "Collision", ecs.OnUpdate, "Position, Obstacle")
ecs.system(bounce, "Bounce", ecs.OnUpdate, "Position, Velocity, Obstacle")
ecs.system(shooted, "Shooted", ecs.OnUpdate, "Position, Obstacle")

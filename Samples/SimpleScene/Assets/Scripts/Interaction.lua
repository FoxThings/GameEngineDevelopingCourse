local ecs = require "ecs"

local function delayed_destroy(it)
    for pos, t, ent in ecs.each(it) do
        t.time = t.time + it.delta_time

        if t.time > 5.0 then
            pos.y = 10000.0
        end
    end
end

local function collision(it)
    for posA, obsA, entA in ecs.each(it) do
        for posB, obsB, entB in ecs.each(it) do
            if entA ~= entB then
                if (posA.x - posB.x) * (posA.x - posB.x) < 5.0 then
                    obsA.isCollided = true
                    obsB.isCollided = true
                else
                    obsA.isCollided = false
                    obsB.isCollided = false
                end
            end
        end
    end
end

local function reverse(it)
    for vel, obs, ent in ecs.each(it) do
        if obs.isCollided then
            vel.x = vel.x * -1.0
        end
    end
end

ecs.system(delayed_destroy, "DelayedDestroy", ecs.OnUpdate, "Position, Timer")
ecs.system(collision, "Collision", ecs.OnUpdate, "Position, Obstacle")
ecs.system(reverse, "Reverse", ecs.OnUpdate, "Velocity, Obstacle")

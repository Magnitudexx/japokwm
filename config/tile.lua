
-- function add(box)
--     ccall((:addBox, corePath), Cvoid, (Cint, Cint, Cint, Cint), 3, 3, 4, 5)
-- end

-- function del()
--     ccall((:del, corePath), Cvoid, (Cint,), 3)
-- end

-- function recurse(arr)
--     a = arr[size(arr)] 
-- end
Direction = {
    TOP = 1,
    BOTTOM = 2,
    LEFT = 3,
    RIGHT = 4,
}

local X<const> = 1
local Y<const> = 2
local WIDTH<const> = 3
local HEIGHT<const> = 4

layoutData = {
    {
        {0, 0, 1, 1},
    },
}

-- set: which window conf set
-- client: current window
function splitContainer(i, j, ratio)
    local i = math.min(i, #layoutData)
    local j = math.min(j, #layoutData[i])
    container = layoutData[i][j]

    print(i, j)
    x = container[X]
    y = container[Y]
    width = container[WIDTH]
    height = container[HEIGHT]

    container[HEIGHT] = height * ratio
    newContainer = {x, y + container[HEIGHT], width, height * (1-ratio)}

    table.insert(layoutData[i], newContainer)
    action.arrangeThis(false)
end

function splitThisContainer(ratio)
    local i = math.max(math.min(info.thisTiledClientCount(), #layoutData), 1)
    local j = math.min(info.thisContainerPosition(), #layoutData[i])
    splitContainer(i, j, ratio)
end

-- set: which window conf set
-- client: current window
function vsplitContainer(i, j, ratio)
    local i = math.min(i, #layoutData)
    local j = math.min(j, #layoutData[i])
    container = layoutData[i][j]

    print(i, j)
    x = container[X]
    y = container[Y]
    width = container[WIDTH]
    height = container[HEIGHT]

    container[WIDTH] = width * ratio
    newContainer = {x + container[WIDTH], y, width * (1-ratio), height}

    table.insert(layoutData[i], newContainer)
    action.arrangeThis(false)
end

function vsplitThisContainer(ratio)
    local i = math.max(math.min(info.thisTiledClientCount(), #layoutData), 1)
    local j = math.min(info.thisContainerPosition(), #layoutData[i])
    vsplitContainer(i, j, ratio)
end

function mergeContainer(i, j1, j2)
    if i > #layoutData then
        return
    end
    if math.max(j1, j2) > #layoutData[i] then
        return
    end

    local i = math.min(i, #layoutData)
    local j1 = math.min(j1, #layoutData[i])
    local j2 = math.min(j2, #layoutData[i])
    local container1 = layoutData[i][j1]
    local container2 = layoutData[i][j2]

    local x = math.min(container1[X], container2[X])
    local y = math.min(container1[Y], container2[Y])
    local width = math.max(container1[X] + container1[WIDTH],
                container2[X] + container2[WIDTH]) - x
    local height = math.max(container1[Y] + container1[HEIGHT],
                container2[Y] + container2[HEIGHT]) - y
    local newContainer = {x, y, width, height}

    layoutData[i][math.min(j1, j2)] = newContainer
    action.arrangeThis(false)
end

function moveContainer(container, n, d)
    local con = container
    if d == Direction.TOP then
        con[Y] = con[Y] - n
    elseif d == Direction.BOTTOM then
        con[Y] = con[Y] + n
    elseif d == Direction.LEFT then
        con[X] = con[X] + n
    elseif d == Direction.RIGHT then
        con[X] = con[X] + n
    end
    return con
end

function moveThisContainer(n, d)
    local i = max(min(thisTiledClientCount(), length(layoutData)), 1)
    local j = min(clientPos(), length(layoutData[i]))
    local container = layoutData[i][j]
    layoutData[i][j] = moveContainer(container, n, d)
    arrangeThis(false)
end

function resizeContainer(container, n, d)
    local con = container
    if d == Direction.TOP then
        con[Y] = con[Y] - n
        con[HEIGHT] = con[HEIGHT] + n
    elseif d == Direction.BOTTOM then
        con[HEIGHT] = con[HEIGHT] + n
    elseif d == Direction.LEFT then
        con[X] = con[X] + n
        con[WIDTH] = con[WIDTH] - n
    elseif d == Direction.RIGHT then
        con[WIDTH] = con[WIDTH] + n
    end
    return con
end

function resizeThisContainer(n, d)
    local i = max(min(thisTiledClientCount(), length(layoutData)), 1)
    local j = min(clientPos(), length(layoutData[i]))
    layoutData[i][j] = resizeContainer(layoutData[i][j], n, d)
    action.arrangeThis(false)
end

function moveResize(container, nmove, nresize, d)
    local con = container
    print("mr:", nmove, nresize)
    con = moveContainer(con, nmove, d)
    con = resizeContainer(con, nresize, d)
    return con
end

-- if d == Direction.LEFT then "raytrace" to the left like that and return the
-- geometry of that area
-- +--------------------------+
-- |< - - - - +---------+     |
-- ||         |         |     |
-- ||    a    |    o    |     |
-- ||         |         |     |
-- |< - - - - +---------+     |
-- +--------------------------+
-- where w is the original window and a is the alternative window
function getAlternativeContainer(container, d)
    local alt = {0, 0, 1, 1}
    if d == Direction.TOP then
        alt[Y] = 0
        alt[HEIGHT] = container[Y]
    elseif d == Direction.BOTTOM then
        alt[Y] = container[Y] + container[HEIGHT]
        alt[HEIGHT] = 1 - container[Y]
    elseif d == Direction.LEFT then
        alt[X] = 0
        alt[WIDTH] = container[X]
    elseif d == Direction.RIGHT then
        alt[X] = container[X] + container[WIDTH]
        alt[WIDTH] = 1 - alt[X]
    end
    return alt
end

-- returns whether container2 is effected
function isEffectedByResizeOf(container, container2, d)
    local resize = false
    if d == Direction.TOP then
        resize = container2[Y] < container[Y]
        local left = container2[X] >= container[X] + container[WIDTH]
        local right = container2[X] + container2[WIDTH] <= container[X]
        resize = resize and not (left or right)
    elseif d == Direction.BOTTOM then
        resize = container2[Y] > container[Y]
        local left = container2[X] >= container[X] + container[WIDTH]
        local right = container2[X] + container2[WIDTH] <= container[X]
        resize = resize and not (left or right)
    elseif d == Direction.LEFT then
        resize = container2[X] < container[X]
        local over = container2[Y] >= container[Y] + container[HEIGHT]
        local under = container2[Y] + container2[HEIGHT] <= container[Y]
        resize = resize and not (over or under)
    elseif d == Direction.RIGHT then
        resize = container[X] > container2[X]
        local over = container2[Y] >= container[Y] + container[HEIGHT]
        local under = container2[Y] + container2[HEIGHT] <= container[Y]
        resize = resize and not (over or under)
    end
    return resize
end

-- finds containers that are effected by the container at i,j
function getResizeEffectedContainers(i, j, d)
    local container = layoutData[i][j]
    local list = {}
    local altCon = getAlternativeContainer(container, d)

    for j2 = 1, #layoutData[i] do
        local con = layoutData[i][j2]
        local resize = false

        if j ~= j2 then
            if isEffectedByResizeOf(con, container, d) then
                print("effected")
                local d = {con[X], con[Y], con[WIDTH], con[HEIGHT], i, j2}
                d[X] = (d[X]-altCon[X])/altCon[WIDTH]
                d[Y] = (d[Y]-altCon[Y])/altCon[HEIGHT]
                d[WIDTH] = d[WIDTH]/altCon[WIDTH]
                d[HEIGHT] = d[HEIGHT]/altCon[HEIGHT]
                table.insert(list, d)
            end
        end
    end
    return list
end


function resizeAll(i, j, n, d)
    local container = layoutData[i][j]
    local resizeContainers = getResizeEffectedContainers(i, j, d)
    layoutData[i][j] = moveResize(container, 0, n, d)
    local altCon = getAlternativeContainer(container, d)

    for k = 1,#resizeContainers do
        print("go")
        local li = resizeContainers[k][5]
        local lj = resizeContainers[k][6]
        layoutData[li][lj][X] = altCon[X] + (resizeContainers[k][X] * altCon[WIDTH])
        layoutData[li][lj][Y] = altCon[Y] + (resizeContainers[k][Y] * altCon[HEIGHT])
        layoutData[li][lj][WIDTH] = resizeContainers[k][WIDTH] * altCon[WIDTH]
        layoutData[li][lj][HEIGHT] = resizeContainers[k][HEIGHT] * altCon[HEIGHT]
    end
end

function resizeMainAll(n, d)
    local i = math.max(math.min(info.thisTiledClientCount(), #layoutData), 1)
    resizeAll(i, 1, n, d)
    action.arrangeThis(false)
end

function resizeThisAll(n, d)
    local i = math.max(math.min(info.thisTiledClientCount(), #layoutData), 1)
    local j = math.min(info.thisContainerPosition(), #layoutData[i])
    resizeAll(i, j, n, d)
    action.arrangeThis(false)
end

function tile()
    layoutData = action.readOverlay("tile")
end

function monocle()
    layoutData = action.readOverlay("monocle")
end

function twoPane()
    layoutData = action.readOverlay("twoPane")
end

function loadLayout(layout)
    layoutData = action.readOverlay(layout)
end

function update(n)
    local i = math.max(math.min(#layoutData, n), 1)
    return layoutData[i]
end

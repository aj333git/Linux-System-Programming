import ScatterLib

let file = "output.txt"

// Swift string → C string
file.withCString { cstr in
    write_scatter_file(cstr)
}


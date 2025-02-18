PROC_FILE = "/proc/myprocfile"

def write_to_proc(data):
    with open(PROC_FILE, "w") as f:
        f.write(data)
    print(f"Written to /proc: {data}")

def read_from_proc():
    with open(PROC_FILE, "r") as f:
        data = f.read()
    print(f"Read from /proc: {data}")

if __name__ == "__main__":
    write_to_proc("Hello, Kernel!")
    read_from_proc()

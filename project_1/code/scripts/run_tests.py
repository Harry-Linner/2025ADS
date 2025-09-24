import subprocess
import os

def run_command(command):
    """
    Runs a command and prints its output.
    """
    try:
        # Use subprocess.run to execute the command
        # shell=True is needed to handle the redirection operator ">"
        subprocess.run(command, shell=True, check=True)
        print(f"Command successful: {command}")
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {command}")
        print(e)
    except FileNotFoundError:
        print(f"Error: Command not found. Make sure your executable path is correct.")

def generate_test_data():
    """
    Generates test data for different sizes and order types.
    """
    # Create the test_data directory if it doesn't exist
    if not os.path.exists("test_data"):
        os.makedirs("test_data")
        print("Created directory: test_data")

    # Define the executable path and data types
    # Note: We use ".." to navigate up one directory level
    # since the script is now in the 'scripts' folder.
    executable_path = r"..\build\random_generator\random_generator.exe"
    order_types = ["inc", "dec", "rand"]
    data_sizes = range(1000, 11000, 1000)

    print("--- Starting data generation ---")

    for order in order_types:
        for size in data_sizes:
            # Construct the full command
            command = f"{executable_path} {size} {order} > ..\\test_data\\input_{size}_{order}.txt"
            print(f"Generating data for N={size}, type={order}...")
            run_command(command)

    print("--- Data generation complete ---")

if __name__ == "__main__":
    generate_test_data()

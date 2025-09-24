import subprocess
import os

def run_command(command, cwd=None):
    """
    Runs a command and prints its output.
    
    Args:
        command (list): A list of command-line arguments.
        cwd (str, optional): The working directory to run the command in.
    """
    try:
        # Use subprocess.run to execute the command, setting the working directory.
        subprocess.run(command, check=True, cwd=cwd)
        print(f"Command successful: {' '.join(command)}")
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {' '.join(command)}")
        print(e)
    except FileNotFoundError:
        print(f"Error: Executable not found. Make sure the path is correct.")

def run_all_tests():
    """
    Runs the tree_analyzer for different data sizes and order types.
    """
    # Get the project root directory
    # Assumes the script is in a 'scripts' subdirectory of the project root
    project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))

    # Define the executable path and test parameters
    # Use the absolute path to the executable for better reliability
    executable_path = os.path.join(project_root, "build", "src", "tree_analyzer.exe")
    order_types = ["inc", "dec", "rand"]
    data_sizes = range(1000, 11000, 1000)

    # Ensure the output file is clean before starting
    output_file_path = os.path.join(project_root, "performance_results.txt")
    if os.path.exists(output_file_path):
        os.remove(output_file_path)
        print(f"Removed previous results file: {output_file_path}")

    print("--- Starting performance analysis ---")

    for order in order_types:
        for size in data_sizes:
            # The tree_analyzer executable will be run from the project_root directory,
            # so it can find 'test_data' relative to that path.
            command = [executable_path, str(size), order]
            print(f"Running analysis for N={size}, type={order}...")
            # Set the working directory (cwd) for the subprocess call
            run_command(command, cwd=project_root)

    print("--- Analysis complete ---")
    print(f"Results are saved in '{output_file_path}'.")

if __name__ == "__main__":
    run_all_tests()

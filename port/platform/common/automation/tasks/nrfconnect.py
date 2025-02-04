import os
import shutil
import sys
from invoke import task
from scripts import u_utils
from scripts.u_log_readers import URttReader
from scripts.u_flags import u_flags_to_cflags, get_cflags_from_u_flags_yml
from scripts.packages import u_package

DEFAULT_CMAKE_DIR = f"{u_utils.UBXLIB_DIR}/port/platform/zephyr/runner"
DEFAULT_BOARD_NAME = "nrf5340dk_nrf5340_cpuapp"
DEFAULT_OUTPUT_NAME = f"runner_{DEFAULT_BOARD_NAME}"
DEFAULT_BUILD_DIR = os.path.join("_build","nrfconnect")

@task()
def check_installation(ctx):
    """Check that the toolchain for nRF connect SDK is installed"""
    ctx.zephyr_pre_command = ""

    # Load required packages
    pkgs = u_package.load(ctx, ["arm_embedded_gcc", "nrfconnectsdk", "make"])
    ncs_pkg = pkgs["nrfconnectsdk"]
    ae_gcc_pkg = pkgs["arm_embedded_gcc"]

    if not u_utils.is_linux():
        # The Zephyr related env variables will be setup by <toolchain>/cmd/env.cmd
        ctx.zephyr_pre_command = f"{ncs_pkg.get_windows_toolchain_path()}/cmd/env.cmd & "
    else:
        ctx.config.run.env["ZEPHYR_BASE"] = f'{ncs_pkg.get_install_path()}/zephyr'
        ctx.config.run.env["ZEPHYR_TOOLCHAIN_VARIANT"] = 'gnuarmemb'
        ctx.config.run.env["GNUARMEMB_TOOLCHAIN_PATH"] = ae_gcc_pkg.get_install_path()


@task(
    pre=[check_installation],
    help={
        "cmake_dir": f"CMake project directory to build (default: {DEFAULT_CMAKE_DIR})",
        "board_name": f"Zephyr board name (default: {DEFAULT_BOARD_NAME})",
        "output_name": f"An output name (build sub folder, default: {DEFAULT_OUTPUT_NAME})",
        "build_dir": f"Output build directory (default: {DEFAULT_BUILD_DIR})",
        "u_flags": "Extra u_flags (when this is specified u_flags.yml will not be used)"
    }
)
def build(ctx, cmake_dir=DEFAULT_CMAKE_DIR, board_name=DEFAULT_BOARD_NAME,
          output_name=DEFAULT_OUTPUT_NAME, build_dir=DEFAULT_BUILD_DIR,
          u_flags=None):
    """Build a nRF connect SDK based application"""
    pristine = "auto"

    # Handle u_flags
    if u_flags:
        ctx.config.run.env["U_FLAGS"] = u_flags_to_cflags(u_flags)
    else:
        # Read U_FLAGS from nrfconnect.u_flags
        u_flags = get_cflags_from_u_flags_yml(ctx.config.vscode_dir, "nrfconnect", output_name)
        ctx.config.run.env["U_FLAGS"] = u_flags["cflags"]
        # If the flags has been modified we trigger a rebuild
        if u_flags['modified']:
            pristine = "always"

    build_dir = os.path.join(build_dir, output_name)
    ctx.run(f'{ctx.zephyr_pre_command}west build -p {pristine} -b {board_name} {cmake_dir} --build-dir {build_dir}')

@task(
    pre=[check_installation],
    help={
        "output_name": f"An output name (build sub folder, default: {DEFAULT_OUTPUT_NAME})",
        "build_dir": f"Output build directory (default: {DEFAULT_BUILD_DIR})"
    }
)
def clean(ctx, output_name=DEFAULT_OUTPUT_NAME, build_dir=DEFAULT_BUILD_DIR):
    """Remove all files for a nRF connect SDK build"""
    build_dir = os.path.join(build_dir, output_name)
    if os.path.exists(build_dir):
        shutil.rmtree(build_dir)

@task(
    pre=[check_installation],
    help={
        "debugger_serial": "The debugger serial number (optional)",
        "output_name": f"An output name (build sub folder, default: {DEFAULT_OUTPUT_NAME}",
        "build_dir": f"Output build directory (default: {DEFAULT_BUILD_DIR})",
        "hex_file": "Optional: Specify the hex file to flash manually"
    }
)
def flash(ctx, debugger_serial="", output_name=DEFAULT_OUTPUT_NAME,
          build_dir=DEFAULT_BUILD_DIR, hex_file=None):
    """Flash a nRF connect SDK based application"""
    build_dir = os.path.abspath(os.path.join(build_dir, output_name))
    if debugger_serial != "":
        debugger_serial = f"--snr {debugger_serial}"
    hex_arg = "" if hex_file == None else f"--hex-file {hex_file}"
    ctx.run(f'{ctx.zephyr_pre_command}west flash --skip-rebuild {hex_arg} -d {build_dir} {debugger_serial} --erase')

@task(
    pre=[check_installation],
)
def log(ctx, mcu="NRF5340_XXAA_APP", debugger_serial=None):
    """Open a log terminal"""
    if debugger_serial == "":
        debugger_serial = None
    with URttReader(mcu, jlink_serial=debugger_serial) as rtt_reader:
        while True:
            data = rtt_reader.read()
            if data:
                text = bytearray(data).decode(sys.stdout.encoding, "backslashreplace")
                sys.stdout.write(text)
                sys.stdout.flush()

@task(
    pre=[check_installation],
)
def terminal(ctx):
    """Open a nRFconnect SDK terminal"""
    ctx.run(f'{ctx.zephyr_pre_command}{ctx.config.run.shell}', pty=True)

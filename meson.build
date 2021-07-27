project(
    'inspur-psu-manager', 'cpp',
    version: '1.0.0',
    meson_version: '>=0.58.0',
    default_options: [
        'warning_level=3',
        'werror=true',
        'cpp_std=c++20',
    ]
)

conf_data = configuration_data()
conf_data.set_quoted('EEPROM_ROOT_PATH', '/sys/bus/i2c/devices')
conf_data.set_quoted('PSU_DRIVER_NAME', 'ipsps1')

configure_file(output: 'config.h',
    configuration: conf_data
)

phosphor_logging_dep = dependency(
    'phosphor-logging',
    fallback: [
        'phosphor-logging',
        'phosphor_logging_dep'
    ],
)
fmt_dep = dependency('fmt', required: false)
if not fmt_dep.found()
  fmt_proj = import('cmake').subproject(
    'fmt',
    cmake_options: [
      '-DCMAKE_POSITION_INDEPENDENT_CODE=ON',
      '-DMASTER_PROJECT=OFF'
    ],
    required: false)
  assert(fmt_proj.found(), 'fmtlib is required')
  fmt_dep = fmt_proj.dependency('fmt')
endif

cpp = meson.get_compiler('cpp')
if cpp.has_header('nlohmann/json.hpp')
    nlohmann_json = declare_dependency()
else
    subproject('nlohmann', required: false)
    nlohmann_json = declare_dependency(
        include_directories: [
            'subprojects/nlohmann/single_include',
            'subprojects/nlohmann/single_include/nlohmann',
        ]
    )
endif

systemd = dependency('systemd')

deps = [
    phosphor_logging_dep,
    nlohmann_json,
    fmt_dep,
    systemd,
]

sources = [
    'psu-main.cpp',
    'psu-manager.cpp',
    'psu-eeprom.cpp',
]

executable(
    'inspur-psuManager',
    sources,
    implicit_include_directories: true,
    dependencies: deps,
    install: true,
    install_dir: get_option('bindir')
)

systemd_system_unit_dir = systemd.get_variable(
        pkgconfig: 'systemdsystemunitdir',
        pkgconfig_define: ['prefix', get_option('prefix')])

configure_file(input: 'services/inspur-psu-manager.service',
                 output: 'inspur-psu-manager.service',
                 copy: true,
                 install_dir: systemd_system_unit_dir)
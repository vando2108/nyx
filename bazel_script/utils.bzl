def create_test_target(srcs, deps):
  for src in srcs:
    base_name, _ = split_filename(src)
    native.cc_test(
      name = base_name,
      srcs = [src],
      deps = [
        "@googletest//:gtest",
        "@googletest//:gtest_main",
      ] + deps,
    )

def create_benchmark_target(srcs, deps):
  for src in srcs:
    base_name, _ = split_filename(src)
    native.cc_binary(
      name = base_name,
      srcs = [src],
      deps = [
        "@google_benchmark//:benchmark",
        "//src/data_structure:data_structure",
      ] + deps,
    )

def split_filename(filename):
  base_name = filename[:filename.rfind('.')]
  extension = filename[filename.rfind('.'):]
  return base_name, extension

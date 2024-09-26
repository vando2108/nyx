load("//bazel_script:create_tags.bzl", "create_tags")

def create_test_target(srcs, deps):
  for src in srcs:
    base_name, _ = split_filename(src)
    native.cc_test(
      name = base_name,
      srcs = [src],
      tags = create_tags(),
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
      tags = create_tags(),
      deps = [
        "@google_benchmark//:benchmark",
        "@com_github_google_glog//:glog",
        "//src/data_structure:data_structure",
        "//src/utils:utils",
      ] + deps,
    )

def split_filename(filename):
  base_name = filename[:filename.rfind('.')]
  extension = filename[filename.rfind('.'):]
  return base_name, extension

def create_tags():
  package_path = native.package_name()
  return ["all"] + package_path.split("/")

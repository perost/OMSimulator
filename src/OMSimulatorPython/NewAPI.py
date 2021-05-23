from OMSimulator import Model, Scope, Types


def newModel(cref: str) -> Model:
  status = Scope._capi.newModel(cref)
  if Types.Status(status) == Types.Status.OK:
    Scope._Scope.append(cref)
  else:
    raise Exception('error {}'.format(Types.Status(status)))
  return Model(cref)

def importFile(file: str) -> Model:
  cref, status = Scope._capi.importFile(file)
  if Types.Status(status) == Types.Status.OK:
    Scope._Scope.append(cref)
  else:
    raise Exception('error {}'.format(Types.Status(status)))
  return Model(cref)

def setTempDirectory(dir: str) -> None:
  status = Scope._capi.setTempDirectory(dir)
  if Types.Status(status) != Types.Status.OK:
    raise Exception('error {}'.format(Types.Status(status)))

def setCommandLineOption(cmd: str) -> None:
  status = Scope._capi.setCommandLineOption(cmd)
  if Types.Status(status) != Types.Status.OK:
    raise Exception('error {}'.format(Types.Status(status)))

def setLoggingLevel(level: int) -> None:
  if not isinstance(level, int):
    raise Exception('bad argument: {}'.format(level))
  status = Scope._capi.setLoggingLevel(level)
  if Types.Status(status) != Types.Status.OK:
    raise Exception('error {}'.format(Types.Status(status)))

def setMaxLogFileSize(size: int) -> None:
  Scope._capi.setMaxLogFileSize(size)

def setLogFile(path: str, size=None) -> None:
  status = Scope._capi.setLogFile(path)
  if Types.Status(status) != Types.Status.OK:
    raise Exception('error {}'.format(Types.Status(status)))
  if size:
    setMaxLogFileSize(size)

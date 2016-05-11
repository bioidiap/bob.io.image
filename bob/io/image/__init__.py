# import Libraries of other lib packages
import bob.io.base

# import our own Library
import bob.extension
bob.extension.load_bob_library('bob.io.image', __file__)

from . import _library
from . import version
from .version import module as __version__

def get_config():
  """Returns a string containing the configuration information.
  """
  import bob.extension
  return bob.extension.get_config(__name__, version.externals)

# gets sphinx autodoc done right - don't remove it
__all__ = [_ for _ in dir() if not _.startswith('_')]

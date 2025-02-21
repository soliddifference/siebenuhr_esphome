import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import time

CONF_MODE = "mode"
CONF_TIME_ID = "time_id"
CONF_TEXT = "text"

from esphome.const import CONF_ID, CONF_MODE

CODEOWNERS = ["@yannicksuter"]

siebenuhr_ns = cg.esphome_ns.namespace("siebenuhr")
SiebenuhrClock = siebenuhr_ns.class_("SiebenuhrClock", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(SiebenuhrClock),
        cv.Optional(CONF_MODE, default=1): cv.int_range(min=0, max=1),
        cv.Optional(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
        cv.Optional(CONF_TEXT, default=""): cv.string,
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var)

    if CONF_TIME_ID in config:
        time_component = await cg.get_variable(config[CONF_TIME_ID])
        cg.add(var.set_time_component(time_component))

    cg.add(var.set_mode(config[CONF_MODE]))
    cg.add(var.set_text(config[CONF_TEXT]))

    cg.add(var.setup())

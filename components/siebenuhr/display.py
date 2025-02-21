import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display # don't remove! probaly related to the fact, that the component is created under "- display:"
from esphome.components import time

CONF_MODE = "mode"
CONF_TIME_ID = "time_id"
CONF_TEXT = "text"

from esphome.const import (CONF_ID, CONF_MODE)

CODEOWNERS = ["@yannicksuter"]

siebenuhr_ns = cg.esphome_ns.namespace("siebenuhr")
SiebenuhrDisplay = siebenuhr_ns.class_("SiebenuhrDisplay", cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(SiebenuhrDisplay),
        cv.Optional(CONF_MODE, default=1): cv.int_range(min=0, max=1),
        cv.Optional(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
        cv.Optional(CONF_TEXT, default=""): cv.string,
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)

    # Pass the `time_id` if provided
    if CONF_TIME_ID in config:
        time_component = await cg.get_variable(config[CONF_TIME_ID])
        cg.add(var.setTimeComponent(time_component))

    cg.add(var.setMode(config[CONF_MODE]))
    cg.add(var.setText(config[CONF_TEXT]))

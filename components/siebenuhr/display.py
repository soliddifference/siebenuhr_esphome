import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display

CONF_MODE = "mode"
from esphome.const import (CONF_ID, CONF_MODE, CONF_LAMBDA)

CODEOWNERS = ["@yannicksuter"]

siebenuhr_ns = cg.esphome_ns.namespace("siebenuhr")
SiebenuhrDisplay = siebenuhr_ns.class_(
    "SiebenuhrDisplay", display.Display
)

CONFIG_SCHEMA = cv.All(
    display.BASIC_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(SiebenuhrDisplay),
            cv.Optional(CONF_MODE, default=1): cv.int_range(min=0, max=1),
        }
    )
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)

    cg.add(var.set_mode(config[CONF_MODE]))

    if lambda_config := config.get(CONF_LAMBDA):
        lambda_ = await cg.process_lambda(
            lambda_config, [(display.DisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))

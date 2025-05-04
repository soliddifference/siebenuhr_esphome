import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, time
from esphome.const import CONF_OUTPUT_ID

CONF_TYPE = "type"
CONF_TIME_ID = "time_id"
CONF_AUTO_BRIGHTNESS = "auto_brightness"
CONF_PERSONALITY = "personality"
CONF_POWER_MONITORING = "power_monitoring"

empty_light_ns = cg.esphome_ns.namespace("siebenuhr")
EmptyLightOutput = empty_light_ns.class_("SiebenuhrClock", light.LightOutput)

CONFIG_SCHEMA = light.BRIGHTNESS_ONLY_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(EmptyLightOutput),
        cv.Required(CONF_TYPE): cv.one_of("REGULAR", "MINI", upper=True),
        cv.Optional(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
        cv.Optional(CONF_AUTO_BRIGHTNESS, default=False): cv.boolean,
        cv.Optional(CONF_POWER_MONITORING, default=False): cv.boolean,
        cv.Required(CONF_PERSONALITY): cv.one_of("SOLIDCOLOR", "COLORWHEEL", "RAINBOW", "MOSAIK", "GLITTER", upper=True),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)

    if CONF_TIME_ID in config:
        time_component = await cg.get_variable(config[CONF_TIME_ID])
        cg.add(var.set_time_component(time_component))

    if CONF_AUTO_BRIGHTNESS in config:
        cg.add(var.set_auto_brightness(config[CONF_AUTO_BRIGHTNESS]))

    if CONF_POWER_MONITORING in config:
        cg.add(var.set_power_monitoring(config[CONF_POWER_MONITORING]))

    if CONF_PERSONALITY in config:
        cg.add(var.set_personality(config[CONF_PERSONALITY]))

    type_map = {
        "REGULAR": 0,
        "MINI": 1,
    }
    cg.add(var.set_type(type_map[config[CONF_TYPE]]))

    cg.add(var.setup())
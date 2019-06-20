const path = require("path");

const withTypescript = require("@zeit/next-typescript");

module.exports = withTypescript({
    webpack: config => {
        config.resolve.alias = {
            ...(config.resolve.alias || {}),
            "~": path.join(__dirname, "./src/")
        };

        return config;
    }
});
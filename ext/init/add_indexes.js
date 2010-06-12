db.events.total.ensureIndex({_key: 1, _event: 1});
db.events.raw.ensureIndex({_key: 1, _uid: -1});
db.events.raw.ensureIndex({_ts: -1});

db.events.sec.ensureIndex({_key: 1, _event: 1});
db.events.sec.ensureIndex({_ts: -1});

db.events.min.ensureIndex({_key: 1, _event:1});
db.events.min.ensureIndex({_ts: -1});

db.events.hour.ensureIndex({_key:1, _event:1});
db.events.hour.ensureIndex({_ts: -1});
db.events.hour.segmented.ensureIndex({_key: 1, _event: 1});
db.events.hour.segmented.ensureIndex({_ts: -1});

db.events.day.ensureIndex({_key: 1, _event: 1});
db.events.day.ensureIndex({_ts: -1});
db.events.day.segmented.ensureIndex({_key: 1, _event:1});
db.events.day.segmented.ensureIndex({_ts: -1});
